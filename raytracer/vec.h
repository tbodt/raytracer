//
//  vec.h
//  raytracer
//
//  Created by Theodore Dubois on 6/8/16.
//  Copyright © 2016 Theodore Dubois. All rights reserved.
//

#ifndef vec_h
#define vec_h

#include <math.h>

#pragma mark Definitions

typedef struct _vec2 {
    float x;
    float y;
} vec2;

typedef struct _vec3 {
    union {
        float x;
        float r;
    };
    union {
        float y;
        float g;
    };
    union {
        float z;
        float b;
    };
} vec3;

typedef struct _vec4 {
    union {
        float x;
        float r;
    };
    union {
        float y;
        float g;
    };
    union {
        float z;
        float b;
    };
    union {
        float w;
        float a;
    };
} vec4;

#pragma mark Constructors

static inline vec2 vec2f(float x, float y) {
    vec2 vec;
    vec.x = x;
    vec.y = y;
    return vec;
}

static inline vec3 vec3f(float x, float y, float z) {
    vec3 vec;
    vec.x = x;
    vec.y = y;
    vec.z = z;
    return vec;
}

static inline vec4 vec4f(float x, float y, float z, float w) {
    vec4 vec;
    vec.x = x;
    vec.y = y;
    vec.z = z;
    vec.w = w;
    return vec;
}

static inline vec4 vec4v3f(vec3 v, float w) {
    vec4 vec;
    vec.x = v.x;
    vec.y = v.y;
    vec.z = v.z;
    vec.w = w;
    return vec;
}

#pragma mark Arithmetic

static inline vec2 add2v(vec2 a, vec2 b) {
    vec2 c;
    c.x = a.x + b.x;
    c.y = a.y + b.y;
    return c;
}

static inline vec3 add3v(vec3 a, vec3 b) {
    vec3 c;
    c.x = a.x + b.x;
    c.y = a.y + b.y;
    c.z = a.z + b.z;
    return c;
}

static inline vec4 add4v(vec4 a, vec4 b) {
    vec4 c;
    c.x = a.x + b.x;
    c.y = a.y + b.y;
    c.z = a.z + b.z;
    c.w = a.w + b.w;
    return c;
}

static inline vec2 sub2v(vec2 a, vec2 b) {
    vec2 c;
    c.x = a.x - b.x;
    c.y = a.y - b.y;
    return c;
}

static inline vec3 sub3v(vec3 a, vec3 b) {
    vec3 c;
    c.x = a.x - b.x;
    c.y = a.y - b.y;
    c.z = a.z - b.z;
    return c;
}

static inline vec4 sub4v(vec4 a, vec4 b) {
    vec4 c;
    c.x = a.x - b.x;
    c.y = a.y - b.y;
    c.z = a.z - b.z;
    c.w = a.w - b.w;
    return c;
}

static inline vec2 mulf2v(float c, vec2 v) {
    v.x *= c;
    v.y *= c;
    return v;
}

static inline vec2 mul2vf(vec2 v, float c) {
    v.x *= c;
    v.y *= c;
    return v;
}

static inline vec3 mulf3v(float c, vec3 v) {
    v.x *= c;
    v.y *= c;
    v.z *= c;
    return v;
}

static inline vec3 mul3vf(vec3 v, float c) {
    v.x *= c;
    v.y *= c;
    v.z *= c;
    return v;
}

static inline vec3 mul3v3v(vec3 a, vec3 b) {
    a.x *= b.x;
    a.y *= b.y;
    a.z *= b.z;
    return a;
}

static inline vec4 mulf4v(float c, vec4 v) {
    v.x *= c;
    v.y *= c;
    v.z *= c;
    v.w *= c;
    return v;
}

static inline vec4 mul4vf(vec4 v, float c) {
    v.x *= c;
    v.y *= c;
    v.z *= c;
    v.w *= c;
    return v;
}

static inline vec2 divf2v(float c, vec2 v) {
    v.x /= c;
    v.y /= c;
    return v;
}

static inline vec2 div2vf(vec2 v, float c) {
    v.x /= c;
    v.y /= c;
    return v;
}

static inline vec3 divf3v(float c, vec3 v) {
    v.x /= c;
    v.y /= c;
    v.z /= c;
    return v;
}

static inline vec3 div3vf(vec3 v, float c) {
    v.x /= c;
    v.y /= c;
    v.z /= c;
    return v;
}

static inline vec4 divf4v(float c, vec4 v) {
    v.x /= c;
    v.y /= c;
    v.z /= c;
    v.w /= c;
    return v;
}

static inline vec4 div4vf(vec4 v, float c) {
    v.x /= c;
    v.y /= c;
    v.z /= c;
    v.w /= c;
    return v;
}

#pragma mark Length/Normalize

static inline float length2v(vec2 v) {
    return sqrtf(v.x*v.x + v.y*v.y);
}

static inline vec2 normalize2v(vec2 v) {
    return div2vf(v, length2v(v));
}

static inline float length3v(vec3 v) {
    return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
}

static inline vec3 normalize3v(vec3 v) {
    return div3vf(v, length3v(v));
}

static inline float length4v(vec4 v) {
    return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z + v.w*v.w);
}

static inline vec4 normalize4v(vec4 v) {
    return div4vf(v, length4v(v));
}

#pragma mark Products

static inline float dot2v(vec2 a, vec2 b) {
    return a.x*b.x + a.y*b.y;
}

static inline float dot3v(vec3 a, vec3 b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

static inline float dot4v(vec4 a, vec4 b) {
    return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
}

static inline vec3 cross(vec3 a, vec3 b) {
    vec3 c;
    c.x = a.y*b.z - b.y*a.z;
    c.y = a.z*b.x - b.z*a.x;
    c.z = a.x*b.y - b.x*a.y;
    return c;
}

#endif /* vec_h */
