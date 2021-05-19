#version 150

in  vec3 normal;
in  vec3 vertexPosition;
in  vec2 texCoord;

out vec4 out_Color;

uniform sampler2D texUnit;
uniform float time;
uniform float textureRepeats;
uniform bool isShaded;
uniform float specularExponent;
uniform vec3 cameraPosition;

#define MAX_LIGHTS 20
uniform bool is_active_light[MAX_LIGHTS];
uniform vec3 lightSourcesDirPosArr[MAX_LIGHTS];
uniform vec3 lightSourcesColorArr[MAX_LIGHTS];
uniform float lightIntensity[MAX_LIGHTS];
uniform float ambientFactor[MAX_LIGHTS];
uniform bool isDirectional[MAX_LIGHTS];

float add_flickering(float light_intensity,float time,int i)
{
    if (int(time+i*3)%61 == 0 || int(time+i*7)%66 == 0) { light_intensity = 0.0; }
    if (int(time+i*13)%5 == 0) { light_intensity = light_intensity/1.1; }
    if (int(time/10.0+i*7)%5 == 0 || int(time/10.0+i*7)%5 == 1) { light_intensity = light_intensity/1.5; }
    return light_intensity;
}

vec3 get_light_direction(bool isDirectional,vec3 lightSourceDirPos,vec3 vertexPosition)
{
    if (isDirectional) {
        return normalize(lightSourceDirPos);
    } else {
        vec3 light_position = lightSourceDirPos;
        return normalize(vertexPosition - light_position);
    }
}

float get_light_distance(bool isDirectional,vec3 lightSourceDirPos,vec3 vertexPosition)
{
    if (isDirectional) {
        return 1.0;
    } else {
        vec3 light_position = lightSourceDirPos;
        return max(0,length(vertexPosition - light_position))/2;
    }
}

vec3 get_diffuse_light(float light_intensity,vec3 normal,vec3 light_direction,vec3 light_colour,float light_distance)
{
    return light_intensity * max(0, -dot(normal, light_direction)) * light_colour * pow(1/light_distance,1.5);
}

vec3 get_specular_light(float light_intensity,vec3 normal,vec3 light_direction,vec3 light_colour,vec3 eyeDirection, float specularExponent)
{
    vec3 reflection = reflect(light_direction, normal);
    return light_intensity * pow(max(0.1, dot(reflection,eyeDirection)), specularExponent) * light_colour/max(1,300/specularExponent)/2;
}

void main(void)
{
    vec4 texturePattern = texture(texUnit, texCoord * textureRepeats);

    vec3 light_ambient = vec3(0.1, 0.1, 0.1);
    vec3 light_diffuse = vec3(0.0, 0.0, 0.0);
    vec3 light_specular = vec3(0.0, 0.0, 0.0);

    for (int i=0; i<MAX_LIGHTS; i++) {
        if (is_active_light[i]) {
            vec3 light_colour = lightSourcesColorArr[i];
            float light_intensity = add_flickering(lightIntensity[i],time,i);
            vec3 light_direction = get_light_direction(isDirectional[i],lightSourcesDirPosArr[i],vertexPosition);
            float light_distance = get_light_distance(isDirectional[i],lightSourcesDirPosArr[i],vertexPosition);
            vec3 eyeDirection = normalize(cameraPosition - vertexPosition);

            light_diffuse += get_diffuse_light(light_intensity,normal,light_direction,light_colour,light_distance);
            light_specular += get_specular_light(light_intensity,normal,light_direction,light_colour,eyeDirection,specularExponent);
        }
    }

    vec3 total_light = light_ambient + light_diffuse + light_specular;

    if(isShaded)
    {
        out_Color = vec4(total_light, 1.0) * texturePattern;
    }
    else
    {
        out_Color = texturePattern;
    }
}
