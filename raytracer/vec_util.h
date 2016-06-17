//
//  vec_util.h
//  raytracer
//
//  Created by Theodore Dubois on 6/15/16.
//  Copyright © 2016 Theodore Dubois. All rights reserved.
//

#ifndef vec_util_h
#define vec_util_h

#include <math.h>

static inline vec4 homogenize(vec3 p) {
    vec4 v;
    v.x = p.x;
    v.y = p.y;
    v.z = p.z;
    v.w = 1;
    return v;
}

static inline vec3 dehomogenize(vec4 p) {
    vec3 v;
    v.x = p.x / p.w;
    v.y = p.y / p.w;
    v.z = p.z / p.w;
    return v;
}

#endif /* vec_util_h */
