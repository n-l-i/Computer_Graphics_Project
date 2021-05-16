#include "user_input.h"

#define MOUSE_START_POSITION_X 200
#define MOUSE_START_POSITION_Y 200

struct user_input_data reset_user_input(struct user_input_data user_input)
{
    user_input.movement = SetVector(0,0,0);
    user_input.rotation = SetVector(0,0,0);

    return user_input;
}

struct user_input_data keyboard_func(struct user_input_data user_input)
{
    GLfloat movement_speed = 0.2;

    if (glutKeyIsDown('w')) { user_input.movement.z += movement_speed; }
    if (glutKeyIsDown('s')) { user_input.movement.z -= movement_speed; }
    if (glutKeyIsDown('a')) { user_input.movement.x -= movement_speed; }
    if (glutKeyIsDown('d')) { user_input.movement.x += movement_speed; }
    if (glutKeyIsDown(' ')) { user_input.movement.y += movement_speed; }
    if (glutKeyIsDown('f')) { user_input.movement.y -= movement_speed; }

    user_input.rotation.x = mouse_x;
    user_input.rotation.y = mouse_y;

    // Resets the mouse position
    glutWarpPointer(MOUSE_START_POSITION_X,MOUSE_START_POSITION_Y);

    return user_input;
}

void mouse_func(int x,int y)
{
    GLfloat turn_speed = 0.5;

    mouse_x = (x-MOUSE_START_POSITION_X)*0.01*turn_speed;
    mouse_y = (y-MOUSE_START_POSITION_Y)*0.01*turn_speed;
}
