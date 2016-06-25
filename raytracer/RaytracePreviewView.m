//
//  RaytracePreviewView.m
//  raytracer
//
//  Created by Theodore Dubois on 6/20/16.
//  Copyright © 2016 Theodore Dubois. All rights reserved.
//

#import "RaytracePreviewView.h"

@interface RaytracePreviewView ()

@property dispatch_queue_t queue;
@property NSTimer *redrawTimer;


@end

@implementation RaytracePreviewView

- (instancetype)initWithScene:(struct scene *)scene {
    if (self = [super init]) {
        self.scene = scene;
        init_raytracer(scene);
        
        self.img = [[NSBitmapImageRep alloc] initWithBitmapDataPlanes:NULL
                                                           pixelsWide:scene->width
                                                           pixelsHigh:scene->height
                                                        bitsPerSample:8
                                                      samplesPerPixel:4
                                                             hasAlpha:YES
                                                             isPlanar:NO
                                                       colorSpaceName:NSCalibratedRGBColorSpace
                                                          bytesPerRow:0
                                                         bitsPerPixel:0];
        for (int y = 0; y < scene->height; y++) {
            for (int x = 0; x < scene->width; x++) {
                [self.img setColor:[NSColor blueColor] atX:x y:y];
            }
        }
        self.redrawTimer = [NSTimer scheduledTimerWithTimeInterval:0.5
                                                            target:self
                                                          selector:@selector(redraw:)
                                                          userInfo:nil
                                                           repeats:YES];
        self.queue = dispatch_queue_create("raytracing", DISPATCH_QUEUE_CONCURRENT);
        
        for (int y = 0; y < scene->height; y++) {
            for (int x = 0; x < scene->width; x++) {
                dispatch_async(self.queue, ^{
                    int nx = scene->width - x - 1;
                    int ny = scene->height - y - 1;
                    vec4 color_vec = raytrace_pixel(nx, ny, scene);
                    if (color_vec.r > 1)
                        color_vec.r = 1;
                    if (color_vec.g > 1)
                        color_vec.g = 1;
                    if (color_vec.b > 1)
                        color_vec.b = 1;
                    @synchronized (self.img) {
                        [self.img setColor:[NSColor colorWithCalibratedRed:color_vec.r
                                                                     green:color_vec.g
                                                                      blue:color_vec.b
                                                                     alpha:color_vec.a]
                                       atX:x y:y];
                    }
                });
            }
        }
    }
    return self;
}

- (void)redraw:(NSTimer *)_ {
    self.needsDisplay = YES;
}

- (void)drawRect:(NSRect)dirtyRect {
    [super drawRect:dirtyRect];
    
    [self.img drawInRect:self.bounds];
}

@end
