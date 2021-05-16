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
struct model_data skybox,ground,walls[MAP_DIM_X*MAP_DIM_Y],lamps[4];
struct light_data lamp_lights[4];
struct camera_data camera;
struct user_input_data user_input;
GLuint textureId = 1;
GLfloat specularExponent[] = {0.0,25.0,50.0,100.0,200.0,400.0};

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
//     init_lights();

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
     corner_a = SetVector(MAP_SCALE*MAP_DIM_X,0,MAP_SCALE*MAP_DIM_Y);
     corner_b = SetVector(-MAP_SCALE*MAP_DIM_X,0-MAP_SCALE,-MAP_SCALE*MAP_DIM_Y);
    strcpy(model_path,"Data/Models/Plane/plane.obj");
    strcpy(tex_path,"Data/Textures/Floor/WoodFloor007.tga");
     texScale = MAP_DIM_X;
    isShaded = 1;
    specExp = 100;

    ground = init_model_data(ground,model_path,tex_path,importMatrix,texScale,isShaded,specExp,isLight,no_light,corner_a,corner_b);

    // init wall models
    int layout[MAP_DIM_Y][MAP_DIM_X];
    read_level_from_file(layout,"Data/Levels/Level_1/layout.txt");
    place_walls(layout,walls);
    
    

    for (int i=0; i<4; ++i)
    {
        vec3 position = SetVector(MAP_SCALE*7*((i%2)*2-1),MAP_SCALE*1,MAP_SCALE*7*(((i*2)-3)/abs((i*2)-3)));
        // init lamp model
        import_trans = T(position.x,position.y,position.z);
        import_rot = Rx(0);
        import_scale = S(MAP_SCALE*0.3f,MAP_SCALE*0.3f,MAP_SCALE*0.3f);
        importMatrix = Mult(import_trans,Mult(import_rot,import_scale));
        corner_a = SetVector(position.x+MAP_SCALE*0.3f/2.0,position.y+MAP_SCALE*0.3f/2.0,position.z+MAP_SCALE*0.3f/2.0);
        corner_b = SetVector(position.x-MAP_SCALE*0.3f/2.0,position.y-MAP_SCALE*0.3f/2.0,position.z-MAP_SCALE*0.3f/2.0);
        strcpy(model_path,"Data/Models/Light_bulb/light_bulb.obj");
        strcpy(tex_path,"Data/Textures/No_texture/no_texture.tga");
        texScale = MAP_DIM_X;
        isShaded = 0;
        specExp = 100;
        int isLight = 1;
        struct light_data lamp_light;

        // init lamp_light
        isActive = 1;
        colour = SetVector(1,1,1);
        intensity = 3.0;
        isDirectional = 0;
        pos_dir = position;

        lamp_light = init_light_data(lamp_light,isActive,colour,intensity,isDirectional,pos_dir);
        lamps[i] = init_model_data(lamps[i],model_path,tex_path,importMatrix,texScale,isShaded,specExp,isLight,lamp_light,corner_a,corner_b);
    }

    projectionMatrix = frustum(left,right,bottom,top,near,far);
    glUniformMatrix4fv(glGetUniformLocation(program,"projectionMatrix"),1,GL_TRUE,projectionMatrix.m);

    printError("init arrays");
}

void display(void) {
    printError("pre display");

    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    time = (GLfloat)glutGet(GLUT_ELAPSED_TIME)*0.001;
    glUniform1f(glGetUniformLocation(program,"time"),time);

    camera = update_camera_position(camera,user_input,walls,ground);
    user_input = reset_user_input(user_input);
    
    
    
    
    
    // display ground model
    load_model_data(ground);
    DrawModel(ground.model,program,"in_Position","in_Normal","in_TexCoord");
    for (int i=0; i<4; ++i)
    {
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
    user_input = keyboard_func(user_input);
}

int main(int argc,char *argv[])
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitContextVersion(3,2);
    glutInitWindowSize(800,800);
    glutCreateWindow ("computer graphics project");
    glutDisplayFunc(display);
    glutPassiveMotionFunc(&mouse_func);
    init ();
    // Timer
    glutTimerFunc(20,&OnTimer,0);
    glutMainLoop();
    return 0;
}
