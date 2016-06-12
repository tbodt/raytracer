//
//  raytrace.c
//  raytracer
//
//  Created by Theodore Dubois on 6/8/16.
//  Copyright © 2016 Theodore Dubois. All rights reserved.
//

#include <stdio.h>
#include <math.h>
#include "raytrace.h"

static struct ray ray_through_pixel(int x, int y, struct scene *s);
static struct intersection find_intersection(struct ray r, struct scene *s);
static vec4 compute_lighting(struct intersection i, struct scene *s);

void init_raytracer(struct scene *s) {
    // all these formulas assume the distance from the camera to the plane is 1
    // it really doesn't matter
    float fovy = (float)s->camera.fov * M_PI / 180; // convert to radians
    s->half_plane_height = tanf(fovy/2);
    s->half_plane_width = s->half_plane_height * (float)s->width / (float)s->height;
    
    s->forward = normalize3v(sub3v(s->camera.eye, s->camera.pos));
    s->sideways = normalize3v(cross(s->camera.up, s->forward));
    s->upward = cross(s->forward, s->sideways);
}

vec4 raytrace_pixel(int x, int y, struct scene *s) {
    struct ray r = ray_through_pixel(x, y, s);
    struct intersection i = find_intersection(r, s);
    return compute_lighting(i, s);
}

static struct ray ray_through_pixel(int x, int y, struct scene *s) {
    struct ray r;
    r.origin = s->camera.pos;
    float plane_x = s->half_plane_width * (x + 0.5 - s->width/2)/(s->width/2);
    float plane_y = s->half_plane_height * (y + 0.5 - s->height/2)/(s->height/2);
    r.direction = normalize3v(add3v(add3v(mulf3v(plane_x, s->sideways), mulf3v(plane_y, s->upward)), s->forward));
    return r;
}

static struct intersection find_intersection(struct ray r, struct scene *s) {
    struct intersection closest;
    closest.distance = INFINITY;
    for (int i = 0; i < s->objects_count; i++) {
        struct object obj = s->objects[i];
        struct intersection intersection = obj.intersect(obj, r, s);
        if (intersection.distance < closest.distance) {
            closest = intersection;
            closest.object = obj;
        }
    }
    return closest;
}

static vec4 compute_lighting(struct intersection isect, struct scene *s) {
    vec4 color = vec4f(0, 0, 0, 0);
    
    if (isect.distance == INFINITY)
        return color;
    
    for (int i = 0; i < s->lights_count; i++) {
        struct light light = s->lights[i];
        
        vec3 light_dir = normalize3v(sub3v(light.position, isect.point));
        float diffuse_num = fmaxf(dot3v(isect.normal, light_dir), 0);
        vec4 diffuse_color = vec4v3f(mulf3v(diffuse_num, light.diffuse_color), 1);
        color = add4v(color, diffuse_color);
    }
    
    return color;
}

struct intersection intersect_sphere(struct object obj, struct ray r, struct scene *s) {
    struct intersection i;
    
    vec3 v = sub3v(r.origin, obj.sphere.center);
    float b = 2 * dot3v(r.direction, v);
    float c = dot3v(v, v) - obj.sphere.radius*obj.sphere.radius;
    
    float dsc = b*b - 4*c;
    if (dsc < 0) {
        i.distance = INFINITY;
        return i;
    }
    
    float x1 = (-b + sqrtf(dsc)) / 2;
    float x2 = (-b - sqrtf(dsc)) / 2;
    if (x1 < 0)
        x1 = INFINITY;
    if (x2 < 0)
        x2 = INFINITY;
            
    i.distance = fminf(x1, x2);
    i.point = add3v(r.origin, mulf3v(i.distance, r.direction));
    i.normal = normalize3v(sub3v(i.point, obj.sphere.center));
    return i;
}
