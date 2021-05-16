#ifndef MODEL_FUNCTIONS_H
#define MODEL_FUNCTIONS_H

    #include <essentials.h>

    struct light_data {
        _Bool isActive;
        vec3 colour;
        GLfloat intensity;
        _Bool isDirectional;
        vec3 pos_dir;
    };

    struct model_data
    {
        Model *model;
        vec3 corner_a;
        vec3 corner_b;
        mat4 modelMatrix;
        float textureScale;
        _Bool isShaded;
        int textureId;
        float specularExponent;
        _Bool isLight;
        struct light_data light;
    };

    struct model_data init_model_data(struct model_data model, char rel_model_path[],
                     char rel_tex_path[], mat4 modelMatrix, float textureScale,
                     _Bool isShaded,float specularExponent, _Bool isLight, struct light_data light, vec3 corner_a, vec3 corner_b);

    struct light_data init_light_data(struct light_data light,_Bool isActive,
                        vec3 colour,GLfloat intensity,_Bool isDirectional,vec3 pos_dir);

    void load_model_data(struct model_data model);

    void load_light(struct light_data light);

    void display_lights();

#endif
