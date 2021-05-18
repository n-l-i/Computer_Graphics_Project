#include <essentials.h>

#include "model_functions.h"
#include "user_input.h"
#include "camera.h"

// Define a frustrum
#define near 1.0
#define far 150.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5

// Init globals
mat4 projectionMatrix;
struct model_data ground,skybox,walls[MAP_DIM_X*MAP_DIM_Y],light_bulbs[MAX_LIGHTS],lamps[MAX_LIGHTS];
struct light_data lamp_lights[MAX_LIGHTS];
struct camera_data camera;
struct user_input_data user_input;
textureId = 1;

void init(void) {
    dumpInfo();

    // GL inits
    glClearColor(0.2,0.2,0.5,0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    printError("GL inits");

    // Load and compile shader
    program = loadShaders("Code/Shaders/main.vert","Code/Shaders/main.frag");
    printError("init shader");

    camera = init_camera(camera);
    user_input = reset_user_input(user_input);

    mat4 import_rot,import_trans,import_scale,importMatrix;
    mat4 rot,trans,scale,transformationMatrix;
    char model_path[PATH_MAX];
    char tex_path[PATH_MAX];
    float texScale;
    _Bool isShaded;
    GLfloat specExp;

    _Bool isActive;
    vec3 colour;
    GLfloat intensity;
    _Bool isDirectional;
    vec3 pos_dir;

    vec3 corner_a,corner_b;

    import_trans = T(0,0,0);
    import_rot = Rx(0);
    import_scale = S(MAP_SCALE*1.0f,MAP_SCALE*1.0f,MAP_SCALE*1.0f);
    importMatrix = Mult(import_trans,Mult(import_rot,import_scale));
    strcpy(model_path,"Data/Models/Lamp/lamp.obj");
    strcpy(tex_path,"Data/Textures/No_texture/no_texture.tga");
    texScale = 1;
    isShaded = 1;
    specExp = 100;

    int isLight = 0;
    struct light_data no_light;

    // init ground model
     import_trans = T(0,0,0);
     import_rot = Rx(0);
     import_scale = S(MAP_SCALE*MAP_DIM_X,MAP_SCALE*1.0f,MAP_SCALE*MAP_DIM_Y);
     importMatrix = Mult(import_trans,Mult(import_rot,import_scale));
     corner_a = SetVector(MAP_SCALE*MAP_DIM_X,0+MAP_SCALE*0.6,MAP_SCALE*MAP_DIM_Y);
     corner_b = SetVector(-MAP_SCALE*MAP_DIM_X,0-MAP_SCALE,-MAP_SCALE*MAP_DIM_Y);
    strcpy(model_path,"Data/Models/Plane/plane.obj");
    strcpy(tex_path,"Data/Textures/Floor/WoodFloor007.tga");
     texScale = MAP_DIM_X;
    isShaded = 1;
    specExp = 300;

    ground = init_model_data(ground,model_path,tex_path,importMatrix,texScale,isShaded,specExp,isLight,no_light,corner_a,corner_b);

    // init skybox
     import_trans = T(0,0,0);
     import_rot = Rx(0);
     import_scale = S(2,2,2);
     importMatrix = Mult(import_trans,Mult(import_rot,import_scale));
     corner_a = SetVector(1,1,1);
     corner_b = SetVector(1,1,1);
    strcpy(model_path,"Data/Models/Sky/sky.obj");
    strcpy(tex_path,"Data/Textures/Sky/Ice001.tga");
     texScale = 3;
    isShaded = 0;
    specExp = 300;

    skybox = init_model_data(skybox,model_path,tex_path,importMatrix,texScale,isShaded,specExp,isLight,no_light,corner_a,corner_b);

    // init wall models
    int layout[MAP_DIM_Y][MAP_DIM_X];
    read_level_from_file(layout,"Data/Levels/Level_1/layout.txt");
    place_walls(layout,walls);

    for (int i=0; i<10; ++i)
    {
        vec3 placement;
        GLfloat rotation;
        if (i == 0) { placement = SetVector(-7,1.5,-7); rotation = 0.5; }
        if (i == 1) { placement = SetVector(-7,1.5,-1); rotation = 0.0; }
        if (i == 2) { placement = SetVector(-4,1.5,1); rotation = 0.75; }
        if (i == 3) { placement = SetVector(-1,1.5,-7); rotation = 0.25; }
        if (i == 4) { placement = SetVector(-1,1.5,-3); rotation = 0.0; }
        if (i == 5) { placement = SetVector(-1,1.5,3); rotation = 0.0; }
        if (i == 6) { placement = SetVector(2,1.5,-1); rotation = 0.25; }
        if (i == 7) { placement = SetVector(7,1.5,-1); rotation = 0.0; }
        if (i == 8) { placement = SetVector(2,1.5,7); rotation = 0.75; }
        if (i == 9) { placement = SetVector(5,1.5,-7); rotation = 0.75; }

        if (rotation == 0.5) { placement = SetVector(placement.x-0.5,placement.y,placement.z); }
        if (rotation == 0.0) { placement = SetVector(placement.x+0.5,placement.y,placement.z); }
        if (rotation == 0.25) { placement = SetVector(placement.x,placement.y,placement.z-0.5); }
        if (rotation == 0.75) { placement = SetVector(placement.x,placement.y,placement.z+0.5); }

        vec3 position = SetVector(MAP_SCALE*placement.x,MAP_SCALE*placement.y,MAP_SCALE*placement.z);
        // init lamp model
        import_trans = T(position.x,position.y,position.z);
        import_rot = Ry(rotation*M_PI*2);
        import_scale = S(MAP_SCALE*0.1f,MAP_SCALE*0.1f,MAP_SCALE*0.1f);
        importMatrix = Mult(import_trans,Mult(import_rot,import_scale));
        corner_a = SetVector(position.x+MAP_SCALE*0.1f,position.y+MAP_SCALE*0.1f,position.z+MAP_SCALE*0.1f);
        corner_b = SetVector(position.x-MAP_SCALE*0.1f,position.y-MAP_SCALE*0.1f,position.z-MAP_SCALE*0.1f);
        strcpy(model_path,"Data/Models/Light_bulb/light_bulb.obj");
        strcpy(tex_path,"Data/Textures/Light_bulb/light_bulb.tga");
        texScale = MAP_DIM_X;
        isShaded = 0;
        specExp = 100;
        int isLight = 1;
        struct light_data lamp_light;

        // init lamp_light
        isActive = 1;
        colour = SetVector(1,1,1);
        intensity = 2.0;
        isDirectional = 0;
//         if (rotation == 0.5) { pos_dir = SetVector(position.x+MAP_SCALE*0.5,position.y,position.z); }
//         if (rotation == 0.0) { pos_dir = SetVector(position.x-MAP_SCALE*0.5,position.y,position.z); }
//         if (rotation == 0.25) { pos_dir = SetVector(position.x,position.y,position.z+MAP_SCALE*0.5); }
//         if (rotation == 0.75) { pos_dir = SetVector(position.x,position.y,position.z-MAP_SCALE*0.5); }
        pos_dir = SetVector(position.x,position.y,position.z);

        lamp_light = init_light_data(lamp_light,isActive,colour,intensity,isDirectional,pos_dir);
        light_bulbs[i] = init_model_data(light_bulbs[i],model_path,tex_path,importMatrix,texScale,isShaded,specExp,isLight,lamp_light,corner_a,corner_b);
        isLight = 0;
        isShaded = 0;
        strcpy(model_path,"Data/Models/Lamp/lamp.obj");
        strcpy(tex_path,"Data/Textures/Lamp/Metal024_light.tga");
        lamps[i] = init_model_data(lamps[i],model_path,tex_path,importMatrix,texScale,isShaded,specExp,isLight,no_light,corner_a,corner_b);
    }

    projectionMatrix = frustum(left,right,bottom,top,near,far);
    glUniformMatrix4fv(glGetUniformLocation(program,"projectionMatrix"),1,GL_TRUE,projectionMatrix.m);

    printError("init arrays");
}

void display(void) {
    printError("pre display");

    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    time = (GLfloat)glutGet(GLUT_ELAPSED_TIME)*0.01;
    glUniform1f(glGetUniformLocation(program,"time"),time);


    // display skybox
    // skybox has different camera matrix so we edit it before it updates again.
    load_model_data(skybox);
	mat4 skyboxCameraMatrix = camera.cameraMatrix;
    skyboxCameraMatrix.m[1*4-1] = 0;
    skyboxCameraMatrix.m[2*4-1] = 0;
    skyboxCameraMatrix.m[3*4-1] = 0;
	glUniformMatrix4fv(glGetUniformLocation(program, "cameraMatrix"), 1, GL_TRUE, skyboxCameraMatrix.m);
    glDisable(GL_DEPTH_TEST);
    DrawModel(skybox.model, program, "in_Position", "in_Normal", "in_TexCoord");
    glEnable(GL_DEPTH_TEST);

    camera = update_camera_position(camera,user_input,walls,ground);
    user_input = reset_user_input(user_input);
    // display ground model
    load_model_data(ground);
    DrawModel(ground.model,program,"in_Position","in_Normal","in_TexCoord");
    // display lamp models
    for (int i=0; i<10; ++i)
    {
        load_model_data(light_bulbs[i]);
        DrawModel(light_bulbs[i].model,program,"in_Position","in_Normal","in_TexCoord");
        load_model_data(lamps[i]);
        DrawModel(lamps[i].model,program,"in_Position","in_Normal","in_TexCoord");
    }
    // display wall models
    for (int i=0; i<MAP_DIM_X*MAP_DIM_Y; ++i)
    {
        if (walls[i].textureId > 0)
        {
            load_model_data(walls[i]);
            DrawModel(walls[i].model,program,"in_Position","in_Normal","in_TexCoord");
        }
    }

    display_lights();

    glutSwapBuffers();
}

void OnTimer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(20,&OnTimer,value);
    user_input = get_user_input(user_input);
}

int main(int argc,char *argv[])
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitContextVersion(3,2);
    glutInitWindowSize(900,900);
    glutCreateWindow ("computer graphics project");
    glutDisplayFunc(display);
    glutPassiveMotionFunc(&mouse_func);
    init ();
    // Timer
    glutTimerFunc(20,&OnTimer,0);
    glutMainLoop();
    return 0;
}
