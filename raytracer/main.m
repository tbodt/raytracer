//
//  main.c
//  raytracer
//
//  Created by Theodore Dubois on 6/8/16.
//  Copyright © 2016 Theodore Dubois-> All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include "raytrace.h"
#include "scene_parser.h"

int main(int argc, const char * argv[]) {
    struct scene *s = parse_scene("die.scene");
    
    clock_t start = clock();
    init_raytracer(s);
    struct color framebuffer[s->height][s->width];
    for (int y = 0; y < s->height; y++) {
        for (int x = 0; x < s->width; x++) {
            vec4 color_vec = raytrace_pixel(x, y, s);
            if (color_vec.r > 1)
                color_vec.r = 1;
            if (color_vec.g > 1)
                color_vec.g = 1;
            if (color_vec.b > 1)
                color_vec.b = 1;
            framebuffer[s->height - y - 1][x].r = (int)(color_vec.r * 255);
            framebuffer[s->height - y - 1][x].g = (int)(color_vec.g * 255);
            framebuffer[s->height - y - 1][x].b = (int)(color_vec.b * 255);
            framebuffer[s->height - y - 1][x].a = (int)(color_vec.a * 255);
        }
    }
    printf("raytracing took %f seconds\n", (clock() - start)/(float)CLOCKS_PER_SEC);
    
    @autoreleasepool {
        [NSApplication sharedApplication];
        unsigned char *planes[] = {(void *) framebuffer};
        NSBitmapImageRep *rep = [[NSBitmapImageRep alloc] initWithBitmapDataPlanes:planes
                                                                        pixelsWide:s->width
                                                                        pixelsHigh:s->height
                                                                     bitsPerSample:8
                                                                   samplesPerPixel:4
                                                                          hasAlpha:YES
                                                                          isPlanar:NO
                                                                    colorSpaceName:NSCalibratedRGBColorSpace
                                                                       bytesPerRow:s->width * 4
                                                                      bitsPerPixel:0];
        NSImage *image = [NSImage new];
        [image addRepresentation:rep];
        [[image TIFFRepresentation] writeToFile:@"~/Desktop/raytraced.png" atomically:NO];
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