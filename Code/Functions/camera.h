#ifndef CAMERA_FUNCTIONS_H
#define CAMERA_FUNCTIONS_H

    #include <essentials.h>
    #include "user_input.h"
    #include "model_functions.h"

    struct camera_data
    {
        vec3 position;
        vec3 target;
        vec3 up;
        mat4 cameraMatrix;
    };

    struct camera_data init_camera(struct camera_data camera);

    struct user_input_data reset_user_input(struct user_input_data user_input);

    struct camera_data update_camera_position(struct camera_data camera,struct user_input_data user_input,struct model_data walls[MAP_DIM_X*MAP_DIM_Y],struct model_data ground);

    _Bool collision_occurs(vec3 position,struct model_data walls[MAP_DIM_X*MAP_DIM_Y],struct model_data ground);

#endif
