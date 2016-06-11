//
//  raytrace.h
//  raytracer
//
//  Created by Theodore Dubois on 6/8/16.
//  Copyright © 2016 Theodore Dubois. All rights reserved.
//

#ifndef raytrace_h
#define raytrace_h

#include "vec.h"

struct scene {
    int width;
    int height;
    
    // camera params
    struct {
        vec3 pos;
        vec3 eye;
        vec3 up;
        float fov;
    } camera;
    
    // objects
    struct object *objects;
    unsigned int objects_count;
    
    // stuff computed before raytracing
    struct {
        float half_plane_width;
        float half_plane_height;
        
        vec3 forward;
        vec3 sideways;
        vec3 upward;
    };
    // more to come
};

struct ray {
    vec3 origin;
    vec3 direction;
};

typedef struct intersection (*intersect_t)(struct object obj, struct ray r, struct scene *s);

struct object {
    union {
        struct sphere {
            vec3 center;
            float radius;
        } sphere;
    };
    intersect_t intersect;
};

struct intersection {
    float distance;
    struct {
        vec3 point;
        struct object object;
    } computed;
};

struct color {
    // all 8 bit unsigned ints
    unsigned int r:8;
    unsigned int g:8;
    unsigned int b:8;
    unsigned int a:8;
};

void init_raytracer(struct scene *s);
struct color raytrace_pixel(int x, int y, struct scene *s);

// intersection functions
struct intersection intersect_sphere(struct object obj, struct ray r, struct scene *s);

#endif /* raytrace_h */
