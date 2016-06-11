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
    s.width = 1000;
    s.height = 1000;
    s.camera.fov = 90;
    s.camera.pos = vec3f(0, 0, 0);
    s.camera.eye = vec3f(0, 0, 1);
    s.camera.up = vec3f(0, 1, 0);
    
    struct object sphere;
    sphere.intersect = intersect_sphere;
    sphere.sphere.radius = 1;
    sphere.sphere.center = vec3f(0, 0, 2);
    
    struct object objects[] = {sphere};
    s.objects = objects;
    s.objects_count = 1;
    
    init_raytracer(&s);
    struct color framebuffer[s.width][s.height];
    for (int y = 0; y < s.height; y++) {
        for (int x = 0; x < s.width; x++) {
            framebuffer[x][y] = raytrace_pixel(x, y, &s);
        }
    }
    
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