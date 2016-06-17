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
#include "vec_util.h"

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
        struct ray rt = r;
        rt.origin = dehomogenize(mul4mv(obj.transform, homogenize(r.origin)));
        struct intersection intersection = obj.intersect(obj, r, s);
        if (intersection.distance < closest.distance) {
            closest = intersection;
            closest.object = obj;
        }
    }
    return closest;
}

static vec4 compute_lighting(struct intersection isect, struct scene *s) {
    vec3 color = vec3f(0, 0, 0);
    
    if (isect.distance == INFINITY)
        return vec4f(0, 0, 0, 1);
    
    vec3 ambient_color = mul3v3v(s->ambient, isect.object.ambient_color);
    color = add3v(color, ambient_color);
    
    for (int i = 0; i < s->lights_count; i++) {
        struct light light = s->lights[i];
        
        vec3 light_dir = normalize3v(sub3v(light.position, isect.point));
        if (dot3v(isect.normal, light_dir) != 0)
            printf("dot product is %f\n", dot3v(isect.normal, light_dir));
        float diffuse_num = fmaxf(dot3v(isect.normal, light_dir), 0);
        vec3 diffuse_color = mulf3v(diffuse_num, mul3v3v(light.diffuse_color, isect.object.diffuse_color));
        color = add3v(color, diffuse_color);
    }
    
    return vec4v3f(color, 1);
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

// möller-trumbore
// pratically copy-pasted from wikipedia
#define EPSILON 0.0001
struct intersection intersect_triangle(struct object obj, struct ray r, struct scene *s) {
    struct intersection i;
    
    vec3 a = obj.triangle.a;
    vec3 b = obj.triangle.b;
    vec3 c = obj.triangle.c;
    
    vec3 e1 = sub3v(b, a);
    vec3 e2 = sub3v(c, a);
    vec3 p = cross(r.direction, e2);
    float det = dot3v(e1, p);
    if (det > -EPSILON && det < EPSILON) {
        i.distance = INFINITY;
        return i;
    }
    
    vec3 t = sub3v(r.origin, a);
    
    float u = dot3v(t, p) / det;
    if (u < 0 || u > 1) {
        i.distance = INFINITY;
        return i;
    }
    
    vec3 q = cross(t, e1);
    
    float v = dot3v(r.direction, q) / det;
    if (v < 0 || u + v > 1) {
        i.distance = INFINITY;
        return i;
    }
    
    float distance = dot3v(e2, q) / det;
    if (distance > EPSILON) {
        i.distance = distance;
        i.point = add3v(r.origin, mulf3v(i.distance, r.direction));
        // it's useful to understand the math
        if (det > 0)
            i.normal = normalize3v(cross(e1, e2));
        else
            i.normal = normalize3v(cross(e2, e1));
    }
    
    return i;
}