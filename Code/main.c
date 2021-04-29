#include <essentials.h>

#include "map_functions.h"
#include "user_input.h"
#include "model_data_functions.h"


GLuint textureId = 1;


// initial camera matrix
vec3 init_p_vector = {-1,2,0};
vec3 init_l_vector = {1,2,-1};
vec3 init_v_vector = {0,1,0};
vec3 p_vector;
vec3 l_vector;
vec3 v_vector;
mat4 cameraMatrix;

GLfloat x_diff = 0;
GLfloat y_diff = 0;

#define near 1.0
#define far 30.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5
GLfloat projectionMatrix[] =
{
    2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,
    0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,
    0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),
    0.0f, 0.0f, -1.0f, 0.0f
};

// light sources
vec3 lightSourcesColorsArr[] =
{
    {1.0f, 0.0f, 0.0f}, // Red light
    {0.0f, 1.0f, 0.0f}, // Green light
    {0.0f, 0.0f, 1.0f}, // Blue light
    {1.0f, 1.0f, 1.0f}  // White light
};

GLint isDirectional[] = {0,0,1,1};

vec3 lightSourcesDirectionsPositions[] =
{
    {10.0f, 5.0f, 0.0f}, // Red light, positional
    {0.0f, 5.0f, 10.0f}, // Green light, positional
    {-1.0f, 0.0f, 0.0f}, // Blue light along X
    {0.0f, 0.0f, -1.0f}  // White light along Z
};

GLfloat specularExponent[] = {0.0, 25.0, 50.0, 100.0, 200.0, 400.0};

struct model_data skybox, ground, walls[dim_x*dim_y];

void OnTimer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(20, &OnTimer, value);
}

void init(void)
{
    dumpInfo();

    // GL inits
    glClearColor(0.2,0.2,0.5,0);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    printError("GL inits");

    // Load and compile shader
    char main_vert_path[PATH_MAX];
    char *r_vert = realpath("Code/Shaders/main.vert", main_vert_path);
    char main_frag_path[PATH_MAX];
    char *r_frag = realpath("Code/Shaders/main.frag", main_frag_path);
    program = loadShaders(main_vert_path, main_frag_path);
    printError("init shader");

    p_vector = init_p_vector;
    l_vector = init_l_vector;
    v_vector = init_v_vector;

    direction_up = SetVector(0.0f,1.0f,0.0f);
    direction_forwards = VectorAdd(p_vector,ScalarMult(l_vector, -1));
    direction_to_the_right = cross(direction_forwards, direction_up);

    glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);

    // initial camera matrix
    cameraMatrix = lookAtv(p_vector, l_vector, v_vector);
    glUniformMatrix4fv(glGetUniformLocation(program, "cameraMatrix"), 1, GL_TRUE, cameraMatrix.m);
    glUniform3fv(glGetUniformLocation(program, "cameraPosition"), 1, &p_vector);

    mouseDrag(prev_1_click.x, prev_1_click.y);

    mat4 import_rot, import_trans, import_scale, importMatrix;
    mat4 rot, trans, scale, transformationMatrix;
    char model_path[PATH_MAX];
    char tex_path[PATH_MAX];
    float texScale;
    _Bool isShaded;
    int specExp;

    trans = T(0.0f, 0.0f, 0.0f);
    rot = Rx(0.0f);
    scale = S(1.0f, 1.0f, 1.0f);
    transformationMatrix = Mult(trans,Mult(rot, scale));

    // init ground model
    import_trans = T(0, 0, 0);
    import_rot = Rx(0);
    import_scale = S(dim_x, 1.0f, dim_y);
    importMatrix = Mult(import_trans,Mult(import_rot, import_scale));
    strcpy(model_path, "Data/Models/Plane/plane.obj");
    strcpy(tex_path, "Data/Textures/No_texture/no_texture.tga");
    texScale = dim_x;
    isShaded = 1;
    specExp = specularExponent[0];

    ground = init_model_data(ground,model_path,tex_path,importMatrix,
                    transformationMatrix,texScale,isShaded,specExp);

    // init wall models
    int layout[dim_y][dim_x];
    read_level_from_file(layout, "Data/Levels/Level_1/layout.txt");
    place_walls(layout, walls);

    printError("init arrays");
}

void display(void)
{
    printError("pre display");

    glUniform3fv(glGetUniformLocation(program, "lightSourcesDirPosArr"), 4, &lightSourcesDirectionsPositions[0].x);
    glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), 4, &lightSourcesColorsArr[0].x);
    glUniform1iv(glGetUniformLocation(program, "isDirectional"), 4, isDirectional);

    GLfloat t = (GLfloat)(glutGet(GLUT_ELAPSED_TIME));
    GLfloat time;

    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mat4 import_rot, import_trans, import_scale, importMatrix;
    mat4 rot, trans, scale, transformationMatrix;
    mat4 modelMatrix;
    GLfloat tx, ty, tz, sx, sy, sz, rx, ry, rz;

    // display ground model
    load_model_data(ground);
    DrawModel(ground.model, program, "in_Position", "in_Normal", "in_TexCoord");
    // display wall models
    for (int i=0; i<dim_x*dim_y; ++i)
    {
        if (walls[i].textureId > 0)
        {
            load_model_data(walls[i]);
            DrawModel(walls[i].model, program, "in_Position", "in_Normal", "in_TexCoord");
        }
    }
    keyPress();

    printError("display");

    glutSwapBuffers();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitContextVersion(3, 2);
    glutCreateWindow ("computer graphics project");
    glutDisplayFunc(display);
    //glutPassiveMotionFunc(mouseHover);
    glutMotionFunc(mouseDrag);
    glutMouseFunc(mouseClick);
    init ();
    // Timer
    glutTimerFunc(20, &OnTimer, 0);
    glutMainLoop();
    return 0;
}
