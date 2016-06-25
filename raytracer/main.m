//
//  main.c
//  raytracer
//
//  Created by Theodore Dubois on 6/8/16.
//  Copyright © 2016 Theodore Dubois. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include "RaytracePreviewView.h"
#include "raytrace.h"
#include "scene_parser.h"

int main(int argc, const char * argv[]) {
    struct scene *s = parse_scene("shadowtest.scene");
    
    @autoreleasepool {
        [NSApplication sharedApplication];
        NSWindow *window = [[NSWindow alloc] initWithContentRect:NSMakeRect(200, 200, s->width, s->height)
                                                       styleMask:NSTitledWindowMask | NSClosableWindowMask
                                                         backing:NSBackingStoreBuffered
                                                           defer:NO];
        RaytracePreviewView *view = [[RaytracePreviewView alloc] initWithScene:s];
        window.contentView = view;
        [window makeKeyAndOrderFront:nil];
        [NSApp activateIgnoringOtherApps:YES];
        printf("activated\n");
        [NSApp run];
    }
    return 0;
}