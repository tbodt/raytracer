//
//  struct raytrace.c
//  struct raytracer
//
//  Created by Theodore Dubois on 6/8/16.
//  Copyright © 2016 Theodore Dubois. All rights reserved.
//

#include <math.h>
#include "raytrace.h"

static void compute_camera(struct scene *s);
static struct ray ray_through_pixel(int x, int y, struct scene *s);

void raytrace(struct scene *s, struct color *framebuffer) {
    compute_camera(s);
    for (int y = 0; y < s->height; y++) {
        for (int x = 0; x < s->width; x++) {
            struct ray r = ray_through_pixel(x, y, s);
            framebuffer[y * s->width + x] = (struct color){0,0,0,255};
        }
    }
}

static void compute_camera(struct scene *s) {
    // all these formulas assume the distance from the camera to the plane is 1
    // it really doesn't matter
    float fovy = (float)s->camera.fov * M_PI / 180; // convert to radians
    s->half_plane_width = tanf(fovy/2);
    s->half_plane_height = s->half_plane_width * (float)s->width / (float)s->height;
    
    s->forward = normalize3v(sub3v(s->camera.pos, s->camera.eye));
    s->sideways = normalize3v(cross(s->forward, s->camera.up));
    s->upward = cross(s->forward, s->sideways);
}

static struct ray ray_through_pixel(int x, int y, struct scene *s) {
    struct ray r;
    r.origin = s->camera.pos;
    float plane_x = s->half_plane_width * (x + 0.5 - s->width/2)/(s->width/2);
    float plane_y = s->half_plane_height * (y + 0.5 - s->height/2)/(s->height/2);
    r.direction = add3v(add3v(mulf3v(plane_x, s->sideways), mulf3v(plane_y, s->upward)), s->forward);
    return r;
}