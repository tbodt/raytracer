//
//  transform.h
//  raytracer
//
//  Created by Theodore Dubois on 6/17/16.
//  Copyright © 2016 Theodore Dubois. All rights reserved.
//

#ifndef transform_h
#define transform_h

#include <math.h>

static inline mat4 translate(float x, float y, float z) {
    return mat4f(1, 0, 0, x,
                 0, 1, 0, y,
                 0, 0, 1, z,
                 0, 0, 0, 1);
}

static inline mat4 scale(float x, float y, float z) {
    return mat4f(x, 0, 0, 0,
                 0, y, 0, 0,
                 0, 0, z, 0,
                 0, 0, 0, 1);
}

static inline mat4 rotate(vec3 axis, float angle) {
    angle *= M_PI/180;
    float cost = cos(angle);
    float icost = 1 - cost;
    float sint = sin(angle);
    float ux = axis.x;
    float uy = axis.y;
    float uz = axis.z;
    
    return mat4f(cost + ux*ux*icost,    ux*uy*icost - uz*sint, ux*uz*icost + uy*sint, 0,
                 uy*ux*icost + uz*sint, cost + uy*uy*icost,    uy*uz*icost - ux*sint, 0,
                 uz*ux*icost - uy*sint, uz*uy*icost + ux*sint, cost + uz*uz*icost,    0,
                 0,                     0,                     0,                     1);
}

#endif /* transform_h */
