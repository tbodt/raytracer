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
static vec4 compute_lighting(struct intersection i, struct scene *s, unsigned depth);
static struct ray bump_ray(struct ray r, vec3 direction);

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
    return compute_lighting(i, s, 0);
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
        rt.origin = dehomogenize(mul4mv(inverse4m(obj.transform), vec4v3f(r.origin, 1)));
        rt.direction = normalize3v(mul3mv(inverse3m(mat3m4(obj.transform)), r.direction));
        
        struct intersection intersection = obj.intersect(obj, rt, s);
        if (intersection.distance < 0 || intersection.distance == INFINITY)
            continue;
        
        intersection.point = dehomogenize(mul4mv(obj.transform, homogenize(intersection.point)));
        intersection.distance = length3v(sub3v(intersection.point, r.origin));
        if (intersection.distance < closest.distance) {
            closest = intersection;
            closest.object = obj;
        }
    }
    closest.normal = normalize3v(mul3mv(transpose3m(inverse3m(mat3m4(closest.object.transform))), closest.normal));
    closest.origin = r.origin;
    return closest;
}

static vec4 compute_lighting(struct intersection isect, struct scene *s, unsigned depth) {
    vec3 color = vec3f(0, 0, 0);
    
    if (isect.distance == INFINITY || depth >= 5)
        return vec4f(0, 0, 0, 1);
    
    vec3 ambient_color = mul3v3v(s->ambient, isect.object.ambient_color);
    color = add3v(color, ambient_color);
    color = add3v(color, isect.object.emissive_color);
    
    vec3 viewer_dir = normalize3v(sub3v(isect.origin, isect.point));
    
    for (int i = 0; i < s->lights_count; i++) {
        struct light light = s->lights[i];
        
        vec3 light_dir;
        float light_distance;
        if (light.type == POINT) {
            light_dir = normalize3v(sub3v(light.position, isect.point));
            light_distance = length3v(sub3v(light.position, isect.point));
        } else {
            light_dir = normalize3v(light.direction);
            light_distance = INFINITY;
        }
        
        if (depth == 0) {
            struct ray shadow_ray;
            shadow_ray.origin = isect.point;
            shadow_ray.direction = light_dir;
            struct intersection shadow_intersection = find_intersection(bump_ray(shadow_ray, isect.normal), s);
            if (shadow_intersection.distance < light_distance)
                continue;
        }
        
        // diffuse
        float diffuse_num = fmaxf(dot3v(isect.normal, light_dir), 0);
        vec3 diffuse_color = mulf3v(diffuse_num, mul3v3v(light.color, isect.object.diffuse_color));
        color = add3v(color, diffuse_color);
        
        // specular
        vec3 half = normalize3v(div3vf(add3v(light_dir, viewer_dir), 2));
        float specular_num = powf(fmaxf(dot3v(isect.normal, half), 0), isect.object.shininess);
        vec3 specular_color = mulf3v(specular_num, mul3v3v(light.color, isect.object.specular_color));
        color = add3v(color, specular_color);
    }
    
    // reflection
    if (isect.object.specular_color.r != 0 || isect.object.specular_color.g != 0 || isect.object.specular_color.b != 0) {
        struct ray reflect_ray;
        reflect_ray.origin = isect.point;
        reflect_ray.direction = normalize3v(sub3v(mulf3v(2*dot3v(viewer_dir, isect.normal), isect.normal), viewer_dir));
        struct intersection reflect_intersection = find_intersection(bump_ray(reflect_ray, isect.normal), s);
        vec3 reflect_color = mul3v3v(vec3v4(compute_lighting(reflect_intersection, s, depth + 1)), isect.object.specular_color);
        color = add3v(color, reflect_color);
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

struct ray bump_ray(struct ray r, vec3 direction) {
    r.origin = add3v(r.origin, mul3vf(direction, 0.01));
    return r;
}