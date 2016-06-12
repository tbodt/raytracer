//
//  main.c
//  raytracer
//
//  Created by Theodore Dubois on 6/8/16.
//  Copyright © 2016 Theodore Dubois. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include "raytrace.h"

int main(int argc, const char * argv[]) {
    struct scene s;
    s.width = 640;
    s.height = 480;
    s.camera.fov = 90;
    s.camera.pos = vec3f(0, 0, 0);
    s.camera.eye = vec3f(0, 0, 1);
    s.camera.up = vec3f(0, 1, 0);
    
    struct object sphere1;
    sphere1.intersect = intersect_sphere;
    sphere1.sphere.radius = 1;
    sphere1.sphere.center = vec3f(0, 1, 2);
    
    struct object sphere2;
    sphere2.intersect = intersect_sphere;
    sphere2.sphere.radius = 1;
    sphere2.sphere.center = vec3f(1, 0, 3);
    
    struct object objects[] = {sphere1, sphere2};
    s.objects = objects;
    s.objects_count = 2;
    
    struct light light1;
    light1.position = vec3f(0, 1, -1);
    light1.ambient_color = vec3f(0, 0, 0);
    light1.diffuse_color = vec3f(0, 1, 1);
    
    struct light lights[] = {light1};
    s.lights = lights;
    s.lights_count = 1;
    
    clock_t start = clock();
    init_raytracer(&s);
    struct color framebuffer[s.height][s.width];
    for (int y = 0; y < s.height; y++) {
        for (int x = 0; x < s.width; x++) {
            vec4 color_vec = raytrace_pixel(x, y, &s);
            framebuffer[s.height - y - 1][x].r = (int)(color_vec.r * 255);
            framebuffer[s.height - y - 1][x].g = (int)(color_vec.g * 255);
            framebuffer[s.height - y - 1][x].b = (int)(color_vec.b * 255);
            framebuffer[s.height - y - 1][x].a = (int)(color_vec.a * 255);
        }
    }
    printf("raytracing took %f seconds", (clock() - start)/(float)CLOCKS_PER_SEC);
    
    @autoreleasepool {
        [NSApplication sharedApplication];
        unsigned char *planes[] = {(void *) framebuffer};
        NSBitmapImageRep *rep = [[NSBitmapImageRep alloc] initWithBitmapDataPlanes:planes
                                                                        pixelsWide:s.width
                                                                        pixelsHigh:s.height
                                                                     bitsPerSample:8
                                                                   samplesPerPixel:4
                                                                          hasAlpha:YES
                                                                          isPlanar:NO
                                                                    colorSpaceName:NSCalibratedRGBColorSpace
                                                                       bytesPerRow:s.width * 4
                                                                      bitsPerPixel:0];
        NSImage *image = [NSImage new];
        [image addRepresentation:rep];
        NSWindow *window = [[NSWindow alloc] initWithContentRect:NSMakeRect(200, 200, rep.size.width, rep.size.height)
                                                       styleMask:NSTitledWindowMask | NSClosableWindowMask
                                                         backing:NSBackingStoreBuffered
                                                           defer:NO];
        NSImageView *imageView = [[NSImageView alloc] initWithFrame:[window contentRectForFrameRect:window.frame]];
        imageView.image = image;
        window.contentView = imageView;
        [window makeKeyAndOrderFront:nil];
        [NSApp run];
    }
    return 0;
}