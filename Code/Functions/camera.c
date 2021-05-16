// Camera struct and functions

#include "camera.h"

struct camera_data init_camera(struct camera_data camera)
{
    camera.position = SetVector(MAP_SCALE*(-7),MAP_SCALE*1,MAP_SCALE*8);
    camera.target = SetVector(MAP_SCALE*(-7),MAP_SCALE*1,MAP_SCALE*(-7));
    camera.up = SetVector(0,1,0);

    return camera;
}

struct camera_data update_camera_position(struct camera_data camera,struct user_input_data user_input,struct model_data walls[MAP_DIM_X*MAP_DIM_Y],struct model_data ground)
{
    // camera rotation
    vec3 looking_direction = Normalize(VectorSub(camera.target,camera.position));
    vec3 x_axis = CrossProduct(looking_direction,SetVector(0,1,0));
    mat4 resulting_rotation = Mult(ArbRotate(x_axis,-user_input.rotation.y),Ry(-user_input.rotation.x));
    looking_direction = MultVec3(resulting_rotation,looking_direction);

    // camera position
    vec3 xz_movement = VectorAdd(ScalarMult(x_axis,user_input.movement.x),ScalarMult(looking_direction,user_input.movement.z));
    vec3 y_movement = SetVector(0,user_input.movement.y,0);
    vec3 resulting_movement = SetVector(xz_movement.x,y_movement.y,xz_movement.z);
    vec3 new_position = VectorAdd(camera.position,resulting_movement);
    if (!collision_occurs(new_position,walls,ground))
    {
        camera.position = new_position;
    }


    // camera target
    camera.target = VectorAdd(camera.position,looking_direction);

    // cameraMatrix
    camera.cameraMatrix = lookAtv(camera.position,camera.target,camera.up);
    glUniformMatrix4fv(glGetUniformLocation(program,"cameraMatrix"),1,GL_TRUE,camera.cameraMatrix.m);

    // We also send the camera position in world coordinates
    glUniform3fv(glGetUniformLocation(program,"cameraPosition"),1,&camera.position);

    return camera;
}

_Bool collision_occurs(vec3 position,struct model_data walls[MAP_DIM_X*MAP_DIM_Y],struct model_data ground)
{
    GLfloat camera_x = position.x;
    GLfloat camera_y = position.y;
    GLfloat camera_z = position.z;
    GLfloat camera_radius = MAP_SCALE/2;

    GLfloat a_x = ground.corner_a.x;
    GLfloat a_y = ground.corner_a.y;
    GLfloat a_z = ground.corner_a.z;
    GLfloat b_x = ground.corner_b.x;
    GLfloat b_y = ground.corner_b.y;
    GLfloat b_z = ground.corner_b.z;
    if ((camera_x-camera_radius < a_x && camera_x+camera_radius > b_x)||(camera_x+camera_radius > a_x && camera_x-camera_radius < b_x))
    {
        if ((camera_z-camera_radius < a_z && camera_z+camera_radius > b_z)||(camera_z+camera_radius > a_z && camera_z-camera_radius < b_z))
        {
            if ((camera_y-camera_radius < a_y && camera_y+camera_radius > b_y)||(camera_y+camera_radius > a_y && camera_y-camera_radius < b_y))
            {
                return 1;
            }
        }
    }
    for(int i=0;i<MAP_DIM_X*MAP_DIM_Y;++i)
    {
        if (walls[i].isShaded)
        {
            a_x = walls[i].corner_a.x;
            a_y = walls[i].corner_a.y;
            a_z = walls[i].corner_a.z;
            b_x = walls[i].corner_b.x;
            b_y = walls[i].corner_b.y;
            b_z = walls[i].corner_b.z;

            if ((camera_x-camera_radius < a_x && camera_x+camera_radius > b_x)||(camera_x+camera_radius > a_x && camera_x-camera_radius < b_x))
            {
                if ((camera_z-camera_radius < a_z && camera_z+camera_radius > b_z)||(camera_z+camera_radius > a_z && camera_z-camera_radius < b_z))
                {
                    if ((camera_y-camera_radius < a_y && camera_y+camera_radius > b_y)||(camera_y+camera_radius > a_y && camera_y-camera_radius < b_y))
                    {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}
