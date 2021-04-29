#include "user_input.h"

float max(float a, float b)
{
    if( a > b ) { return a; }
    return b;
}
float min(float a, float b)
{
    if( a < b ) { return a; }
    return b;
}

void keyPress()
{
    vec3 movement = {0.0f,0.0f,0.0f};
    //  move forwards
    if (glutKeyIsDown('w'))
    {
        vec3 w_movement = ScalarMult(direction_forwards, -1.0f/10.0f);
        movement = VectorAdd(movement, SetVector(w_movement.x, 0, w_movement.z));
    }
    //  move backwards
    if (glutKeyIsDown('s'))
    {
        vec3 s_movement = ScalarMult(direction_forwards, 1.0f/10.0f);
        movement = VectorAdd(movement, SetVector(s_movement.x, 0, s_movement.z));
    }
    //  move left
    if (glutKeyIsDown('a'))
    {
        movement = VectorAdd(movement, ScalarMult(direction_to_the_right, 1.0f/10.0f));
    }
    //  move right
    if (glutKeyIsDown('d'))
    {
        movement = VectorAdd(movement, ScalarMult(direction_to_the_right, -1.0f/10.0f));
    }
    //  move up
    if (glutKeyIsDown(' '))
    {
        movement = VectorAdd(movement, ScalarMult(direction_up, 1.0f/10.0f));
    }
    //  move down
    if (glutKeyIsDown('f'))
    {
        movement = VectorAdd(movement, ScalarMult(direction_up, -1.0f/10.0f));
    }

    p_vector = VectorAdd(p_vector, movement);
    l_vector = VectorAdd(l_vector, movement);
    cameraMatrix = lookAtv(p_vector, l_vector, v_vector);
    glUniformMatrix4fv(glGetUniformLocation(program, "cameraMatrix"), 1, GL_TRUE, cameraMatrix.m);
    glUniform3fv(glGetUniformLocation(program, "cameraPosition"), 1, &p_vector);

    direction_forwards = VectorAdd(p_vector,ScalarMult(l_vector, -1));
    direction_to_the_right = cross(direction_forwards, direction_up);
}


void mouseDrag(int x, int y)
{
    // camera position
    x_diff += -(x - prev_1_click.x)/100;
    y_diff += -(y - prev_1_click.y)/100;
    y_diff = min(max(y_diff, -0.99), 0.99);
    prev_2_click.x = prev_1_click.x;
    prev_2_click.y = prev_1_click.y;
    prev_1_click.x = x;
    prev_1_click.y = y;
    clicked_previously = 0;

    vec3 y_movement = SetVector(0, sin(y_diff*M_PI/2), 0);
    vec3 x_movement = SetVector(sin(x_diff*M_PI/2), 0, sin(M_PI/2+x_diff*M_PI/2));

    vec3 new_l = VectorAdd(VectorAdd(x_movement,y_movement), p_vector);
    l_vector = new_l;

    cameraMatrix = lookAtv(p_vector, l_vector, v_vector);
    glUniformMatrix4fv(glGetUniformLocation(program, "cameraMatrix"), 1, GL_TRUE, cameraMatrix.m);
    glUniform3fv(glGetUniformLocation(program, "cameraPosition"), 1, &p_vector);

    direction_forwards = VectorAdd(p_vector,ScalarMult(l_vector, -1));
    direction_to_the_right = cross(direction_forwards, direction_up);
}

void mouseClick(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        prev_2_click.x = prev_1_click.x;
        prev_2_click.y = prev_1_click.y;
        prev_1_click.x = x;
        prev_1_click.y = y;
        if( prev_2_click.x == x && prev_2_click.y == y && clicked_previously == 1 )
        {
            x_diff = 0;
            y_diff = 0;

            float new_px = 2*sin(acos(init_p_vector.x/2.0f)+2.0f*M_PI*1.0f + M_PI/2);
            float new_py = init_p_vector.y;
            float new_pz = 2*sin(asin(init_p_vector.z/2.0f)+2.0f*M_PI*1.0f);

            float new_lx = 2*sin(acos(init_l_vector.x/2.0f)+2.0f*M_PI*1.0f + M_PI/2);
            float new_ly = init_l_vector.y;
            float new_lz = 2*sin(asin(init_l_vector.z/2.0f)+2.0f*M_PI*1.0f);

            p_vector = SetVector(new_px,new_py,new_pz);
            l_vector = SetVector(new_lx,new_ly,new_lz);
            v_vector = init_v_vector;

            cameraMatrix = lookAtv(p_vector, l_vector, v_vector);
            glUniformMatrix4fv(glGetUniformLocation(program, "cameraMatrix"), 1, GL_TRUE, cameraMatrix.m);
            glUniform3fv(glGetUniformLocation(program, "cameraPosition"), 1, &p_vector);

            direction_forwards = VectorAdd(p_vector,ScalarMult(l_vector, -1));
            direction_to_the_right = cross(direction_forwards, direction_up);
            mouseDrag(prev_1_click.x, prev_1_click.y);
        }
    clicked_previously = 1;
    }
}
