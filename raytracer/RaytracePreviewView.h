//
//  RaytracePreviewView.h
//  raytracer
//
//  Created by Theodore Dubois on 6/20/16.
//  Copyright © 2016 Theodore Dubois. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include "raytrace.h"

@interface RaytracePreviewView : NSView

- (instancetype)initWithScene:(struct scene *)scene;

@property struct scene *scene;
@property NSBitmapImageRep *img;

@end
