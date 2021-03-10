
// Should work as is on Linux and Mac. MS Windows needs GLEW or glee.
// See separate Visual Studio version of my demos.
#ifdef __APPLE__
    #include <OpenGL/gl3.h>
    // Linking hint for Lightweight IDE
    // uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include <math.h>

#include <stdio.h>
#include "LittleOBJLoader.h"

#include <string.h>
#include <limits.h>
#include <stdlib.h>




// Reference to shader program
GLuint program;

GLuint textureId = 1;

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

void OnTimer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(20, &OnTimer, value);
}

// map dimensions
#define dim_x 10
#define dim_y 10

// initial camera matrix
GLfloat x_diff = 0;
GLfloat y_diff = 0;

vec3 p_vector = {0,23,1};
vec3 l_vector = {0,0,0};
vec3 v_vector = {0,1,0};
mat4 cameraMatrix;

vec3 direction_forwards, direction_up, direction_to_the_right;

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

vec2 prev_1_click;
vec2 prev_2_click;
_Bool clicked_previously;

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

            p_vector = SetVector(2*sin(-M_PI/4 + M_PI/2),2,2*sin(-M_PI/4));
            l_vector = SetVector(2*sin(-M_PI/4 + M_PI/2)+1,2+0,2*sin(-M_PI/4)+0);
            v_vector = SetVector(0,1,0);

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

void readLevelFromFile(int layout[dim_y][dim_x], char filepath[])
{
    int rows = dim_y;
    int cols = dim_x;
    int row=0;
    int col=0;
    char file_path[PATH_MAX];
    char *r_file = realpath("Data/Levels/Level_1/layout.txt", file_path);
    int c;
    FILE *file;
    file = fopen(file_path, "r");
    if (file) {
        while ((c = getc(file)) != EOF)
        {
            // getting the number corresponding to the ASCII value
            // and checking that the char is a number between 0 and 9
            int val = (int)c-(int)'0';
            if (val >= 0 && val < 10) {
                layout[row][col] = val;
                row++;
            }
            if(c == '\n' && row > 0){
                col++;
                row=0;
            }
        }
        fclose(file);
    }
}


struct model_data
{
    Model *model;
    mat4 transformationMatrix;
    mat4 importMatrix;
    float textureScale;
    _Bool isShaded;
    int textureId;
    int specularExponent;
};

struct model_data skybox, ground, walls[dim_x*dim_y];

struct model_data init_model_data(struct model_data model, char rel_model_path[],
                     char rel_tex_path[], mat4 importMatrix,
                     mat4 transformationMatrix, float textureScale,
                     _Bool isShaded,int specularExponent)
{
    char abs_model_path[PATH_MAX];
    char *r_model = realpath(rel_model_path, abs_model_path);
    char abs_tex_path[PATH_MAX];
    char *r_tex = realpath(rel_tex_path, abs_tex_path);
    model.model = LoadModel(abs_model_path);
    LoadTGATextureSimple(abs_tex_path, &textureId);
    model.textureId = textureId++;
    model.textureScale = textureScale;
    model.transformationMatrix = transformationMatrix;
    model.importMatrix = importMatrix;
    model.isShaded = isShaded;
    model.specularExponent = specularExponent;
    return model;
}

void load_model_data(struct model_data model)
{
    mat4 modelMatrix = Mult(model.importMatrix,model.transformationMatrix);
    glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_TRUE, modelMatrix.m);
    glBindTexture(GL_TEXTURE_2D, model.textureId);
    glUniform1f(glGetUniformLocation(program, "textureRepeats"), model.textureScale);
    glUniform1i(glGetUniformLocation(program, "isShaded"), model.isShaded);
    glUniform1f(glGetUniformLocation(program, "specularExponent"), model.specularExponent);
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

    direction_up = SetVector(0.0f,1.0f,0.0f);
    direction_forwards = VectorAdd(p_vector,ScalarMult(l_vector, -1));
    direction_to_the_right = cross(direction_forwards, direction_up);

    glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);

    // initial camera matrix
    cameraMatrix = lookAtv(p_vector, l_vector, v_vector);
    glUniformMatrix4fv(glGetUniformLocation(program, "cameraMatrix"), 1, GL_TRUE, cameraMatrix.m);
    glUniform3fv(glGetUniformLocation(program, "cameraPosition"), 1, &p_vector);

    //mouseDrag(prev_1_click.x, prev_1_click.y);

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

    // ground model
    import_trans = T(0, 0, 0);
    import_rot = Rx(0);
    import_scale = S((dim_x-1)/2.0f, 1.0f, (dim_y-1)/2.0f);
    importMatrix = Mult(import_trans,Mult(import_rot, import_scale));
    strcpy(model_path, "Data/Models/Plane/plane.obj");
    strcpy(tex_path, "Data/Textures/No_texture/no_texture.tga");
    texScale = (dim_x-1)/2.0f;
    isShaded = 1;
    specExp = specularExponent[0];
    
    ground = init_model_data(ground,model_path,tex_path,importMatrix,
                    transformationMatrix,texScale,isShaded,specExp);
    
    // wall models
    strcpy(model_path, "Data/Models/Wall/wall.obj");
    strcpy(tex_path, "Data/Textures/No_texture/no_texture.tga");
    texScale = 50;
    isShaded = 1;
    specExp = specularExponent[0];
    
    
    
    int rows = dim_y;
    int cols = dim_x;
    int layout[rows][cols];
    readLevelFromFile(layout, "Data/Levels/Level_1/layout.txt");
    int wall_index = 0;
    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            int val = layout[col][row];
            if (val == 1 || val == 2)
            {
                import_trans = T(1*col-(cols-1)/2.0f, 0, 1*row-(rows-1)/2.0f);
                import_rot = Ry((val*0.25f)*M_PI*2);
                import_scale = S(1.0f, 1.0f, 1.0f);
                importMatrix = Mult(import_trans,Mult(import_rot, import_scale));

                walls[wall_index] = init_model_data(walls[wall_index],model_path,tex_path,importMatrix,
                transformationMatrix,texScale,isShaded,specExp);
                
                wall_index++;
            }
        }
    }

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

    // ground model
    load_model_data(ground);
    DrawModel(ground.model, program, "in_Position", "in_Normal", "in_TexCoord");
    // wall models
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
