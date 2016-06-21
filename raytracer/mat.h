//
//  mat.h
//  raytracer
//
//  Created by Theodore Dubois on 6/14/16.
//  Copyright © 2016 Theodore Dubois. All rights reserved.
//

#ifndef mat_h
#define mat_h

#include <math.h>

typedef struct {
    union {
        float a[3][3];
//        vec3 r[3];
    };
} mat3;

typedef struct {
    union {
        float a[4][4];
//        vec4 r[4];
    };
} mat4;

static inline mat3 mat3f(float a11, float a12, float a13,
           float a21, float a22, float a23,
           float a31, float a32, float a33) {
    mat3 m;
    m.a[0][0] = a11;
    m.a[0][1] = a12;
    m.a[0][2] = a13;
    m.a[1][0] = a21;
    m.a[1][1] = a22;
    m.a[1][2] = a23;
    m.a[2][0] = a31;
    m.a[2][1] = a32;
    m.a[2][2] = a33;
    return m;
}

static inline mat3 identity3m() {
    mat3 i;
    i.a[0][0] = 1;
    i.a[0][1] = 0;
    i.a[0][2] = 0;
    i.a[1][0] = 0;
    i.a[1][1] = 1;
    i.a[1][2] = 0;
    i.a[2][0] = 0;
    i.a[2][1] = 0;
    i.a[2][2] = 1;
    return i;
}

static inline mat4 identity4m() {
    mat4 i;
    i.a[0][0] = 1;
    i.a[0][1] = 0;
    i.a[0][2] = 0;
    i.a[0][3] = 0;
    i.a[1][0] = 0;
    i.a[1][1] = 1;
    i.a[1][2] = 0;
    i.a[1][3] = 0;
    i.a[2][0] = 0;
    i.a[2][1] = 0;
    i.a[2][2] = 1;
    i.a[2][3] = 0;
    i.a[3][0] = 0;
    i.a[3][1] = 0;
    i.a[3][2] = 0;
    i.a[3][3] = 1;
    return i;
}

static inline mat4 mat4f(float a11, float a12, float a13, float a14,
           float a21, float a22, float a23, float a24,
           float a31, float a32, float a33, float a34,
           float a41, float a42, float a43, float a44) {
    mat4 m;
    m.a[0][0] = a11;
    m.a[0][1] = a12;
    m.a[0][2] = a13;
    m.a[0][3] = a14;
    m.a[1][0] = a21;
    m.a[1][1] = a22;
    m.a[1][2] = a23;
    m.a[1][3] = a24;
    m.a[2][0] = a31;
    m.a[2][1] = a32;
    m.a[2][2] = a33;
    m.a[2][3] = a34;
    m.a[3][0] = a41;
    m.a[3][1] = a42;
    m.a[3][2] = a43;
    m.a[3][3] = a44;
    return m;
}

static inline mat3 mat3m4(mat4 f) {
    mat3 m;
    m.a[0][0] = f.a[0][0];
    m.a[0][1] = f.a[0][1];
    m.a[0][2] = f.a[0][2];
    m.a[1][0] = f.a[1][0];
    m.a[1][1] = f.a[1][1];
    m.a[1][2] = f.a[1][2];
    m.a[2][0] = f.a[2][0];
    m.a[2][1] = f.a[2][1];
    m.a[2][2] = f.a[2][2];
    return m;
}

static inline mat3 add3m(mat3 a, mat3 b) {
    a.a[0][0] += b.a[0][0];
    a.a[0][1] += b.a[0][1];
    a.a[0][2] += b.a[0][2];
    a.a[1][0] += b.a[1][0];
    a.a[1][1] += b.a[1][1];
    a.a[1][2] += b.a[1][2];
    a.a[2][0] += b.a[2][0];
    a.a[2][1] += b.a[2][1];
    a.a[2][2] += b.a[2][2];
    return a;
}

static inline mat4 add4m(mat4 a, mat4 b) {
    a.a[0][0] += b.a[0][0];
    a.a[0][1] += b.a[0][1];
    a.a[0][2] += b.a[0][2];
    a.a[0][3] += b.a[0][3];
    a.a[1][0] += b.a[1][0];
    a.a[1][1] += b.a[1][1];
    a.a[1][2] += b.a[1][2];
    a.a[1][3] += b.a[1][3];
    a.a[2][0] += b.a[2][0];
    a.a[2][1] += b.a[2][1];
    a.a[2][2] += b.a[2][2];
    a.a[2][3] += b.a[2][3];
    a.a[3][0] += b.a[3][0];
    a.a[3][1] += b.a[3][1];
    a.a[3][2] += b.a[3][2];
    a.a[3][3] += b.a[3][3];
    return a;
}

static inline mat3 sub3m(mat3 a, mat3 b) {
    a.a[0][0] -= b.a[0][0];
    a.a[0][1] -= b.a[0][1];
    a.a[0][2] -= b.a[0][2];
    a.a[1][0] -= b.a[1][0];
    a.a[1][1] -= b.a[1][1];
    a.a[1][2] -= b.a[1][2];
    a.a[2][0] -= b.a[2][0];
    a.a[2][1] -= b.a[2][1];
    a.a[2][2] -= b.a[2][2];
    return a;
}

static inline mat4 sub4m(mat4 a, mat4 b) {
    a.a[0][0] -= b.a[0][0];
    a.a[0][1] -= b.a[0][1];
    a.a[0][2] -= b.a[0][2];
    a.a[0][3] -= b.a[0][3];
    a.a[1][0] -= b.a[1][0];
    a.a[1][1] -= b.a[1][1];
    a.a[1][2] -= b.a[1][2];
    a.a[1][3] -= b.a[1][3];
    a.a[2][0] -= b.a[2][0];
    a.a[2][1] -= b.a[2][1];
    a.a[2][2] -= b.a[2][2];
    a.a[2][3] -= b.a[2][3];
    a.a[3][0] -= b.a[3][0];
    a.a[3][1] -= b.a[3][1];
    a.a[3][2] -= b.a[3][2];
    a.a[3][3] -= b.a[3][3];
    return a;
}

static inline mat3 mul3m(mat3 a, mat3 b) {
    mat3 c;
    c.a[0][0] = a.a[0][0]*b.a[0][0] + a.a[0][1]*b.a[1][0] + a.a[0][2]*b.a[2][0];
    c.a[0][1] = a.a[0][0]*b.a[0][1] + a.a[0][1]*b.a[1][1] + a.a[0][2]*b.a[2][1];
    c.a[0][2] = a.a[0][0]*b.a[0][2] + a.a[0][1]*b.a[1][2] + a.a[0][2]*b.a[2][2];
    c.a[1][0] = a.a[1][0]*b.a[0][0] + a.a[1][1]*b.a[1][0] + a.a[1][2]*b.a[2][0];
    c.a[1][1] = a.a[1][0]*b.a[0][1] + a.a[1][1]*b.a[1][1] + a.a[1][2]*b.a[2][1];
    c.a[1][2] = a.a[1][0]*b.a[0][2] + a.a[1][1]*b.a[1][2] + a.a[1][2]*b.a[2][2];
    c.a[2][0] = a.a[2][0]*b.a[0][0] + a.a[2][1]*b.a[1][0] + a.a[2][2]*b.a[2][0];
    c.a[2][1] = a.a[2][0]*b.a[0][1] + a.a[2][1]*b.a[1][1] + a.a[2][2]*b.a[2][1];
    c.a[2][2] = a.a[2][0]*b.a[0][2] + a.a[2][1]*b.a[1][2] + a.a[2][2]*b.a[2][2];
    return c;
}

static inline mat4 mul4m(mat4 a, mat4 b) {
    mat4 c;
    c.a[0][0] = a.a[0][0]*b.a[0][0] + a.a[0][1]*b.a[1][0] + a.a[0][2]*b.a[2][0] + a.a[0][3]*b.a[3][0];
    c.a[0][1] = a.a[0][0]*b.a[0][1] + a.a[0][1]*b.a[1][1] + a.a[0][2]*b.a[2][1] + a.a[0][3]*b.a[3][1];
    c.a[0][2] = a.a[0][0]*b.a[0][2] + a.a[0][1]*b.a[1][2] + a.a[0][2]*b.a[2][2] + a.a[0][3]*b.a[3][2];
    c.a[0][3] = a.a[0][0]*b.a[0][3] + a.a[0][1]*b.a[1][3] + a.a[0][2]*b.a[2][3] + a.a[0][3]*b.a[3][3];
    c.a[1][0] = a.a[1][0]*b.a[0][0] + a.a[1][1]*b.a[1][0] + a.a[1][2]*b.a[2][0] + a.a[1][3]*b.a[3][0];
    c.a[1][1] = a.a[1][0]*b.a[0][1] + a.a[1][1]*b.a[1][1] + a.a[1][2]*b.a[2][1] + a.a[1][3]*b.a[3][1];
    c.a[1][2] = a.a[1][0]*b.a[0][2] + a.a[1][1]*b.a[1][2] + a.a[1][2]*b.a[2][2] + a.a[1][3]*b.a[3][2];
    c.a[1][3] = a.a[1][0]*b.a[0][3] + a.a[1][1]*b.a[1][3] + a.a[1][2]*b.a[2][3] + a.a[1][3]*b.a[3][3];
    c.a[2][0] = a.a[2][0]*b.a[0][0] + a.a[2][1]*b.a[1][0] + a.a[2][2]*b.a[2][0] + a.a[2][3]*b.a[3][0];
    c.a[2][1] = a.a[2][0]*b.a[0][1] + a.a[2][1]*b.a[1][1] + a.a[2][2]*b.a[2][1] + a.a[2][3]*b.a[3][1];
    c.a[2][2] = a.a[2][0]*b.a[0][2] + a.a[2][1]*b.a[1][2] + a.a[2][2]*b.a[2][2] + a.a[2][3]*b.a[3][2];
    c.a[2][3] = a.a[2][0]*b.a[0][3] + a.a[2][1]*b.a[1][3] + a.a[2][2]*b.a[2][3] + a.a[2][3]*b.a[3][3];
    c.a[3][0] = a.a[3][0]*b.a[0][0] + a.a[3][1]*b.a[1][0] + a.a[3][2]*b.a[2][0] + a.a[3][3]*b.a[3][0];
    c.a[3][1] = a.a[3][0]*b.a[0][1] + a.a[3][1]*b.a[1][1] + a.a[3][2]*b.a[2][1] + a.a[3][3]*b.a[3][1];
    c.a[3][2] = a.a[3][0]*b.a[0][2] + a.a[3][1]*b.a[1][2] + a.a[3][2]*b.a[2][2] + a.a[3][3]*b.a[3][2];
    c.a[3][3] = a.a[3][0]*b.a[0][3] + a.a[3][1]*b.a[1][3] + a.a[3][2]*b.a[2][3] + a.a[3][3]*b.a[3][3];
    return c;
}

static inline vec3 mul3mv(mat3 m, vec3 v) {
    vec3 a;
    a.x = m.a[0][0]*v.x + m.a[0][1]*v.y + m.a[0][2]*v.z;
    a.y = m.a[1][0]*v.x + m.a[1][1]*v.y + m.a[1][2]*v.z;
    a.z = m.a[2][0]*v.x + m.a[2][1]*v.y + m.a[2][2]*v.z;
    return a;
}

static inline vec3 mul3vm(vec3 v, mat3 m) {
    return mul3mv(m, v);
}

static inline vec4 mul4mv(mat4 m, vec4 v) {
    vec4 a;
    a.x = m.a[0][0]*v.x + m.a[0][1]*v.y + m.a[0][2]*v.z + m.a[0][3]*v.w;
    a.y = m.a[1][0]*v.x + m.a[1][1]*v.y + m.a[1][2]*v.z + m.a[1][3]*v.w;
    a.z = m.a[2][0]*v.x + m.a[2][1]*v.y + m.a[2][2]*v.z + m.a[2][3]*v.w;
    a.w = m.a[3][0]*v.x + m.a[3][1]*v.y + m.a[3][2]*v.z + m.a[3][3]*v.w;
    return a;
}

static inline vec4 mul4vm(vec4 v, mat4 m) {
    return mul4mv(m, v);
}

#pragma mark oh god

static inline mat3 inverse3m(mat3 m) {
    // again, based on wikipedia
    float a = m.a[0][0];
    float b = m.a[0][1];
    float c = m.a[0][2];
    float d = m.a[1][0];
    float e = m.a[1][1];
    float f = m.a[1][2];
    float g = m.a[2][0];
    float h = m.a[2][1];
    float i = m.a[2][2];
    
    float A = e*i - f*h;
    float B = c*h - b*i;
    float C = b*f - c*e;
    float D = f*g - d*i;
    float E = a*i - c*g;
    float F = c*d - a*f;
    float G = d*h - e*g;
    float H = b*g - a*h;
    float I = a*e - b*d;
    
    float idet = 1/(a*A + b*D + c*G);
    
    return mat3f(A*idet, B*idet, C*idet,
                 D*idet, E*idet, F*idet,
                 G*idet, H*idet, I*idet);
}

// very based on linmath.h
static inline mat4 inverse4m(mat4 q) {
    // this came from asking wolfram-alpha to symbolically invert a 4x4 matrix
    
    float a = q.a[0][0];
    float b = q.a[0][1];
    float c = q.a[0][2];
    float d = q.a[0][3];
    float e = q.a[1][0];
    float f = q.a[1][1];
    float g = q.a[1][2];
    float h = q.a[1][3];
    float i = q.a[2][0];
    float j = q.a[2][1];
    float k = q.a[2][2];
    float l = q.a[2][3];
    float m = q.a[3][0];
    float n = q.a[3][1];
    float o = q.a[3][2];
    float p = q.a[3][3];
    
    // brute force determinant
    float idet = 1/(a*f*k*p - a*f*l*o - a*g*j*p + a*g*l*n + a*h*j*o - a*h*k*n -
                    b*e*k*p + b*e*l*o + b*g*i*p - b*g*l*m - b*h*i*o + b*h*k*m +
                    c*e*j*p - c*e*l*n - c*f*i*p + c*f*l*m + c*h*i*n - c*h*j*m -
                    d*e*j*o + d*e*k*n + d*f*i*o - d*f*k*m - d*g*i*n + d*g*j*m);
    
    float A = -h*k*n + g*l*n + h*j*o - f*l*o - g*j*p + f*k*p;
    float B =  d*k*n - c*l*n - d*j*o + b*l*o + c*j*p - b*k*p;
    float C = -d*g*n + c*h*n + d*f*o - b*h*o - c*f*p + b*g*p;
    float D =  d*g*j - c*h*j - d*f*k + b*h*k + c*f*l - b*g*l;
    float E =  h*k*m - g*l*m - h*i*o + e*l*o + g*i*p - e*k*p;
    float F = -d*k*m + c*l*m + d*i*o - a*l*o - c*i*p + a*k*p;
    float G =  d*g*m - c*h*m - d*e*o + a*h*o + c*e*p - a*g*p;
    float H = -d*g*i + c*h*i + d*e*k - a*h*k - c*e*l + a*g*l;
    float I = -h*j*m + f*l*m + h*i*n - e*l*n - f*i*p + e*j*p;
    float J =  d*j*m - b*l*m - d*i*n + a*l*n + b*i*p - a*j*p;
    float K = -d*f*m + b*h*m + d*e*n - a*h*n - b*e*p + a*f*p;
    float L =  d*f*i - b*h*i - d*e*j + a*h*j + b*e*l - a*f*l;
    float M =  g*j*m - f*k*m - g*i*n + e*k*n + f*i*o - e*j*o;
    float N = -c*j*m + b*k*m + c*i*n - a*k*n - b*i*o + a*j*o;
    float O =  c*f*m - b*g*m - c*e*n + a*g*n + b*e*o - a*f*o;
    float P = -c*f*i + b*g*i + c*e*j - a*g*j - b*e*k + a*f*k;
    
    return mat4f(A*idet,B*idet,C*idet,D*idet,
                 E*idet,F*idet,G*idet,H*idet,
                 I*idet,J*idet,K*idet,L*idet,
                 M*idet,N*idet,O*idet,P*idet);
}

static inline mat3 transpose3m(mat3 m) {
    mat3 t;
    t.a[0][0] = m.a[0][0];
    t.a[0][1] = m.a[1][0];
    t.a[0][2] = m.a[2][0];
    t.a[1][0] = m.a[0][1];
    t.a[1][1] = m.a[1][1];
    t.a[1][2] = m.a[2][1];
    t.a[2][0] = m.a[0][2];
    t.a[2][1] = m.a[1][2];
    t.a[2][2] = m.a[2][2];
    return t;
}

static inline mat4 transpose4m(mat4 m) {
    mat4 t;
    t.a[0][0] = m.a[0][0];
    t.a[0][1] = m.a[1][0];
    t.a[0][2] = m.a[2][0];
    t.a[0][3] = m.a[3][0];
    t.a[1][0] = m.a[0][1];
    t.a[1][1] = m.a[1][1];
    t.a[1][2] = m.a[2][1];
    t.a[1][3] = m.a[3][1];
    t.a[2][0] = m.a[0][2];
    t.a[2][1] = m.a[1][2];
    t.a[2][2] = m.a[2][2];
    t.a[2][3] = m.a[3][2];
    t.a[3][0] = m.a[0][3];
    t.a[3][1] = m.a[1][3];
    t.a[3][2] = m.a[2][3];
    t.a[3][3] = m.a[3][3];
    return t;
}

#endif /* mat_h */
