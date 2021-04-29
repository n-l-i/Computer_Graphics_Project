#ifndef MODEL_DATA_FUNCTIONS_H
#define MODEL_DATA_FUNCTIONS_H

    #include <essentials.h>

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

    struct model_data init_model_data(struct model_data model, char rel_model_path[],
                     char rel_tex_path[], mat4 importMatrix,
                     mat4 transformationMatrix, float textureScale,
                     _Bool isShaded,int specularExponent);
    void load_model_data(struct model_data model);

#endif
