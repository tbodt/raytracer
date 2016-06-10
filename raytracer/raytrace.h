//
//  raytrace.h
//  raytracer
//
//  Created by Theodore Dubois on 6/8/16.
//  Copyright © 2016 Theodore Dubois. All rights reserved.
//

#include "vec.h"
#include "scene.h"

#ifndef raytrace_h
#define raytrace_h

struct color {
    // all 8 bit unsigned ints
    unsigned int r:8;
    unsigned int g:8;
    unsigned int b:8;
    unsigned int a:8;
};

void raytrace(struct scene *s, struct color *framebuffer);

struct ray {
    vec3 origin;
    vec3 direction;
};

#endif /* raytrace_h */
