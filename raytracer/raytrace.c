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

void init_raytracer(struct scene *s) {
    // all these formulas assume the distance from the camera to the plane is 1
    // it really doesn't matter
    float fovy = (float)s->camera.fov * M_PI / 180; // convert to radians
    s->half_plane_width = tanf(fovy/2);
    s->half_plane_height = s->half_plane_width * (float)s->width / (float)s->height;
    
    s->forward = normalize3v(sub3v(s->camera.eye, s->camera.pos));
    s->sideways = normalize3v(cross(s->forward, s->camera.up));
    s->upward = cross(s->forward, s->sideways);
}

struct color raytrace_pixel(int x, int y, struct scene *s) {
    struct ray r = ray_through_pixel(x, y, s);
    struct intersection i = find_intersection(r, s);
    if (i.distance == INFINITY)
        return (struct color) {0,0,0,255};
    else
        return (struct color) {255,255,255,255};
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
            closest.computed.object = obj;
        }
    }
    return closest;
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
    float x = fminf(fmaxf(x1, 0), fmaxf(x2, 0));
    
    if (x == 0) {
        x = INFINITY;
    }
    i.distance = x;
    return i;
}

