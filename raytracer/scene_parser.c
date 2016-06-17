//
//  scene->parser.c
//  raytracer
//
//  Created by Theodore Dubois on 6/12/16.
//  Copyright © 2016 Theodore Dubois. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "scene_parser.h"

struct scene *parse_scene(char *file) {
    FILE *f = fopen(file, "r");
    struct scene *scene = malloc(sizeof(struct scene));
    char line[1000]; // safe maximum
    char *components[15]; // also probably safe maximum
    
    vec3 *vertices = NULL;
    int vertex_count = 0;
    
    vec3 current_ambient = vec3f(0, 0, 0);
    
    struct object *objects = malloc(1000 * sizeof(struct object)); // yet another safe maximum
    int objects_count = 0;
    
    while (fgets(line, sizeof(line), f) != NULL) {
        char *newline = line + strlen(line) - 1;
        if (*newline == '\n')
            *newline = '\0';
            
        if (line[0] == '#')
            continue;
        
        char *p = line;
        int num_components = 0;
        while (1) {
            components[num_components++] = p;
            char *space = strchr(p, ' ');
            if (space == NULL)
                break;
            *space = '\0';
            p = space + 1;
            while (*p == ' ')
                p++;
        }
        
        if (num_components == 1)
            continue;
        
        if (strcmp(components[0], "size") == 0) {
            scene->width = atoi(components[1]);
            scene->height = atoi(components[2]);
        } else if (strcmp(components[0], "camera") == 0) {
            scene->camera.pos = vec3f(atof(components[1]), atof(components[2]), atof(components[3]));
            scene->camera.eye = vec3f(atof(components[4]), atof(components[5]), atof(components[6]));
            scene->camera.up = vec3f(atof(components[7]), atof(components[8]), atof(components[9]));
            scene->camera.fov = atof(components[10]);
        } else if (strcmp(components[0], "maxverts") == 0) {
            vertices = malloc(atoi(components[1]) * sizeof(vec3));
        } else if (strcmp(components[0], "vertex") == 0) {
            if (vertices == NULL)
                abort();
            vertices[vertex_count++] = vec3f(atof(components[1]), atof(components[2]), atof(components[3]));
        } else if (strcmp(components[0], "ambient") == 0) {
            current_ambient = vec3f(atof(components[1]), atof(components[2]), atof(components[3]));
        } else if (strcmp(components[0], "tri") == 0) {
            objects[objects_count].triangle.a = vertices[atoi(components[1])];
            objects[objects_count].triangle.b = vertices[atoi(components[2])];
            objects[objects_count].triangle.c = vertices[atoi(components[3])];
            objects[objects_count].ambient_color = current_ambient;
            objects[objects_count].diffuse_color = current_ambient;
            objects[objects_count].intersect = intersect_triangle;
            objects_count++;
        } else if (strcmp(components[0], "sphere") == 0) {
            objects[objects_count].sphere.center = vec3f(atof(components[1]), atof(components[2]), atof(components[3]));
            objects[objects_count].sphere.radius = atof(components[4]);
            objects[objects_count].ambient_color = current_ambient;
            objects[objects_count].diffuse_color = current_ambient;
            objects[objects_count].intersect = intersect_sphere;
            objects_count++;
        }
    }
    
    scene->objects = objects;
    scene->objects_count = objects_count;
    
    scene->lights_count = 0;
    scene->ambient = vec3f(0.1, 0.1, 0.1);
    
    fclose(f);
    return scene;
}