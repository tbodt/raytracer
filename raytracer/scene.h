//
//  scene.h
//  raytracer
//
//  Created by Theodore Dubois on 6/8/16.
//  Copyright © 2016 Theodore Dubois. All rights reserved.
//

#include "vec.h"

#ifndef scene_h
#define scene_h

struct scene {
    int width;
    int height;
    
    // camera params
    struct {
        vec3 pos;
        vec3 eye;
        vec3 up;
        float fov;
    } camera;
    
    // stuff computed before raytracing
    struct {
        float half_plane_width;
        float half_plane_height;
        
        vec3 forward;
        vec3 sideways;
        vec3 upward;
    };
    // more to come
};

#endif /* scene_h */
