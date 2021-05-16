#ifndef ESSENTIALS_H
#define ESSENTIALS_H

    #include "MicroGlut.h"
    #include "GL_utilities.h"
    #include "LittleOBJLoader.h"

    #include <math.h>
    #include <stdio.h>
    #include <string.h>
    #include <limits.h>
    #include <stdlib.h>

    #include "misc_functions.h"

    // map dimensions
    #define MAP_DIM_X 10
    #define MAP_DIM_Y 10

    // map scale
    #define MAP_SCALE 5

    // max number of lights
    #define MAX_LIGHTS 4

    // Reference to shader program
    GLuint program;

    GLuint textureId;
    GLfloat time;

    GLfloat mouse_x;
    GLfloat mouse_y;

#endif
