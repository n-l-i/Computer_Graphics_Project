#ifndef USER_INPUT_H
#define USER_INPUT_H

    #include <essentials.h>

    struct user_input_data
    {
        vec3 movement;
        vec3 rotation;
    };

    struct user_input_data get_user_input(struct user_input_data user_input);

    void mouse_func(int x,int y);

#endif
