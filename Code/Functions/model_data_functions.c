#include "model_data_functions.h"

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
    model.specularExponent = specularExponent;
    // models using no_texture.tga shouldn't be affected by lighting
    if (strstr(abs_tex_path,"no_texture.tga")) { model.isShaded = 0; }
    else { model.isShaded = isShaded; }

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
