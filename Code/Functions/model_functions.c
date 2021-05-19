#include "model_functions.h"

struct model_data init_model_data(struct model_data model, char rel_model_path[],
                    char rel_tex_path[], mat4 modelMatrix, float textureScale,
                    _Bool isShaded,float specularExponent, _Bool isLight,
                    struct light_data light, vec3 corner_a, vec3 corner_b)
{
    char abs_model_path[PATH_MAX];
    char *r_model = realpath(rel_model_path, abs_model_path);
    char abs_tex_path[PATH_MAX];
    char *r_tex = realpath(rel_tex_path, abs_tex_path);
    model.model = LoadModel(abs_model_path);
    LoadTGATextureSimple(abs_tex_path, &textureId);
    model.textureId = textureId++;
    model.textureScale = textureScale;
    model.modelMatrix = modelMatrix;
    model.specularExponent = max(1,specularExponent);
    // models using no_texture.tga shouldn't be affected by lighting
    if (strstr(abs_tex_path,"no_texture.tga")) { model.isShaded = isShaded; }
    else { model.isShaded = isShaded; }
    model.isLight = isLight;
    model.light = light;
    model.corner_a = corner_a;
    model.corner_b = corner_b;

    return model;
}


struct light_data init_light_data(struct light_data light,_Bool is_active_light,
                    vec3 colour,GLfloat intensity,_Bool isDirectional,vec3 pos_dir)
{
    light.is_active_light = is_active_light;
    light.colour = colour;
    light.intensity = intensity;
    light.isDirectional = isDirectional;
    light.pos_dir = pos_dir;

    return light;
}

void load_model_data(struct model_data model)
{
    mat4 modelMatrix = model.modelMatrix;
    glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_TRUE, modelMatrix.m);
    glBindTexture(GL_TEXTURE_2D, model.textureId);
    glUniform1f(glGetUniformLocation(program, "textureRepeats"), model.textureScale);
    glUniform1i(glGetUniformLocation(program, "isShaded"), model.isShaded);
    glUniform1f(glGetUniformLocation(program, "specularExponent"), model.specularExponent);

    if (model.isLight)
    {
        load_light(model.light);
    }
}

GLint is_active_light[MAX_LIGHTS];
GLfloat lightSourcesColorsArr[MAX_LIGHTS*3];
GLfloat lightIntensity[MAX_LIGHTS];
GLint isDirectional[MAX_LIGHTS];
GLfloat lightSourcesDirectionsPositions[MAX_LIGHTS*3];

void load_light(struct light_data light)
{
    if (!light.is_active_light) { return; }

    for (int i=0; i<MAX_LIGHTS; i++)
    {
        if (!is_active_light[i]){
            is_active_light[i] = light.is_active_light;
            lightIntensity[i] = light.intensity;
            isDirectional[i] = light.isDirectional;

            lightSourcesColorsArr[i*3+0] = light.colour.x;
            lightSourcesColorsArr[i*3+1] = light.colour.y;
            lightSourcesColorsArr[i*3+2] = light.colour.z;

            lightSourcesDirectionsPositions[i*3+0] = light.pos_dir.x;
            lightSourcesDirectionsPositions[i*3+1] = light.pos_dir.y;
            lightSourcesDirectionsPositions[i*3+2] = light.pos_dir.z;

            break;
        }

    }

}

void display_lights()
{
    glUniform1iv(glGetUniformLocation(program, "is_active_light"), MAX_LIGHTS, is_active_light);
    glUniform3fv(glGetUniformLocation(program, "lightSourcesDirPosArr"), MAX_LIGHTS, lightSourcesDirectionsPositions);
    glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), MAX_LIGHTS, lightSourcesColorsArr);
    glUniform1fv(glGetUniformLocation(program, "lightIntensity"), MAX_LIGHTS, lightIntensity);
    glUniform1iv(glGetUniformLocation(program, "isDirectional"), MAX_LIGHTS, isDirectional);

    for (int i=0; i<MAX_LIGHTS; i++)
    {
        is_active_light[i] = 0;
    }
}
