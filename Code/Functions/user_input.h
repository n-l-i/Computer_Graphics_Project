#ifndef USER_INPUT_H
#define USER_INPUT_H

    #include <essentials.h>

    float max(float a, float b);
    float min(float a, float b);

    // initial camera matrix
    vec3 init_p_vector;
    vec3 init_l_vector;
    vec3 init_v_vector;
    vec3 p_vector;
    vec3 l_vector;
    vec3 v_vector;
    mat4 cameraMatrix;

    GLfloat x_diff;
    GLfloat y_diff;

    vec3 direction_forwards, direction_up, direction_to_the_right;
    vec2 prev_1_click, prev_2_click;
    _Bool clicked_previously;

    void keyPress();
    void mouseDrag(int x, int y);
    void mouseClick(int button, int state, int x, int y);

#endif
