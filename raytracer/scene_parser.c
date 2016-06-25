//
//  scene->parser.c
//  raytracer
//
//  Created by Theodore Dubois on 6/12/16.
//  Copyright © 2016 Theodore Dubois. All rights reserved.
//

/************************************************************
 ***************** THIS CODE IS GARBAGE *********************
 ***************** YOU HAVE BEEN WARNED *********************
 ************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "scene_parser.h"
#include "transform.h"

struct scene *parse_scene(char *file) {
    FILE *f = fopen(file, "r");
    if (f == NULL) {
        perror(file);
        exit(2);
    }
    struct scene *scene = malloc(sizeof(struct scene));
    char line[1000]; // safe maximum
    char *components[15]; // also probably safe maximum
    
    vec3 *vertices = NULL;
    int vertex_count = 0;
    
    struct object *objects = malloc(100000 * sizeof(struct object)); // yet another safe maximum
    int objects_count = 0;
    
    struct light *lights = malloc(10 * sizeof(struct light));
    int lights_count = 0;
    
    vec3 current_ambient = vec3f(0, 0, 0);
    vec3 current_emissive = vec3f(0, 0, 0);
    vec3 current_diffuse = vec3f(0, 0, 0);
    vec3 current_specular = vec3f(0, 0, 0);
    float current_shininess = 1;
    
    mat4 transform_stack[10]; // should be big enough
    int transform_sp = 0;
    transform_stack[0] = identity4m();
    
    while (fgets(line, sizeof(line), f) != NULL) {
        char *newline = line + strlen(line) - 1;
        if (*newline == '\n')
            *newline = '\0';
            
        if (line[0] == '#')
            continue;
        
        char *p = line;
        int num_components = 0;
        char *component;
        while ((component = strsep(&p, " ")) != NULL) {
            if (component[0] != '\0')
                components[num_components++] = component;
        }
        if (num_components == 0)
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
        } else if (strcmp(components[0], "emission") == 0) {
            current_emissive = vec3f(atof(components[1]), atof(components[2]), atof(components[3]));
        } else if (strcmp(components[0], "diffuse") == 0) {
            current_diffuse = vec3f(atof(components[1]), atof(components[2]), atof(components[3]));
        } else if (strcmp(components[0], "specular") == 0) {
            current_specular = vec3f(atof(components[1]), atof(components[2]), atof(components[3]));
        } else if (strcmp(components[0], "shininess") == 0) {
            current_shininess = atof(components[1]);
        } else if (strcmp(components[0], "tri") == 0) {
            objects[objects_count].triangle.a = vertices[atoi(components[1])];
            objects[objects_count].triangle.b = vertices[atoi(components[2])];
            objects[objects_count].triangle.c = vertices[atoi(components[3])];
            objects[objects_count].transform = transform_stack[transform_sp];
            objects[objects_count].ambient_color = current_ambient;
            objects[objects_count].emissive_color = current_emissive;
            objects[objects_count].diffuse_color = current_diffuse;
            objects[objects_count].specular_color = current_specular;
            objects[objects_count].shininess = current_shininess;
            objects[objects_count].intersect = intersect_triangle;
            objects_count++;
        } else if (strcmp(components[0], "sphere") == 0) {
            objects[objects_count].sphere.center = vec3f(atof(components[1]), atof(components[2]), atof(components[3]));
            objects[objects_count].sphere.radius = atof(components[4]);
            objects[objects_count].transform = transform_stack[transform_sp];
            objects[objects_count].ambient_color = current_ambient;
            objects[objects_count].emissive_color = current_emissive;
            objects[objects_count].diffuse_color = current_diffuse;
            objects[objects_count].specular_color = current_specular;
            objects[objects_count].shininess = current_shininess;
            objects[objects_count].intersect = intersect_sphere;
            objects_count++;
        } else if (strcmp(components[0], "point") == 0) {
            lights[lights_count].position = vec3f(atof(components[1]), atof(components[2]), atof(components[3]));
            lights[lights_count].color = vec3f(atof(components[4]), atof(components[5]), atof(components[6]));
            lights[lights_count].type = POINT;
            lights_count++;
        } else if (strcmp(components[0], "directional") == 0) {
            lights[lights_count].position = vec3f(atof(components[1]), atof(components[2]), atof(components[3]));
            lights[lights_count].color = vec3f(atof(components[4]), atof(components[5]), atof(components[6]));
            lights[lights_count].type = DIRECTIONAL;
            lights_count++;
        } else if (strcmp(components[0], "pushTransform") == 0) {
            transform_stack[transform_sp + 1] = transform_stack[transform_sp];
            transform_sp++;
        } else if (strcmp(components[0], "popTransform") == 0) {
            transform_sp--;
        } else if (strcmp(components[0], "translate") == 0) {
            transform_stack[transform_sp] = mul4m(transform_stack[transform_sp], translate(atof(components[1]), atof(components[2]), atof(components[3])));
        } else if (strcmp(components[0], "scale") == 0) {
            transform_stack[transform_sp] = mul4m(transform_stack[transform_sp], scale(atof(components[1]), atof(components[2]), atof(components[3])));
        } else if (strcmp(components[0], "rotate") == 0) {
            transform_stack[transform_sp] = mul4m(transform_stack[transform_sp], rotate(vec3f(atof(components[1]), atof(components[2]), atof(components[3])), atof(components[4])));
        }
    }
    
    scene->objects = objects;
    scene->objects_count = objects_count;
    
    scene->lights = lights;
    scene->lights_count = lights_count;
    scene->ambient = vec3f(1, 1, 1);
    
    fclose(f);
    return scene;
}