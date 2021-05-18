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
uniform bool isActive[MAX_LIGHTS];
uniform vec3 lightSourcesDirPosArr[MAX_LIGHTS];
uniform vec3 lightSourcesColorArr[MAX_LIGHTS];
uniform float lightIntensity[MAX_LIGHTS];
uniform float ambientFactor[MAX_LIGHTS];
uniform bool isDirectional[MAX_LIGHTS];



void main(void)
{
    vec4 texturePattern = texture(texUnit, texCoord * textureRepeats);


    vec3 light_ambient = vec3(0.1, 0.1, 0.1);
    vec3 light_diffuse = vec3(0.0, 0.0, 0.0);
    vec3 light_specular = vec3(0.0, 0.0, 0.0);

    for (int i=0; i<MAX_LIGHTS; i++) {
        if (isActive[i] && int(time+i*3)%61 != 0 && int(time+i*7)%81 != 0) {
            vec3 light_colour = lightSourcesColorArr[i];
            float light_intensity = lightIntensity[i];
            if (int(time+i*13)%5 == 0) { light_intensity = light_intensity/1.1; }
            if (int(time/10.0+i*7)%5 == 0 || int(time/10.0+i*7)%5 == 1) { light_intensity = light_intensity/1.5; }
            vec3 light_direction;
            float light_distance;

            if (isDirectional[i]) {
                light_direction = normalize(lightSourcesDirPosArr[i]);
                light_distance = 0.1;
            } else {
                vec3 light_position = lightSourcesDirPosArr[i];
                light_direction = normalize(vertexPosition - light_position);
                light_distance = max(0,length(vertexPosition - light_position))/2;
            }

            // Diffuse light component
            light_diffuse += light_intensity * max(0, -dot(normal, light_direction)) * light_colour * pow(1/light_distance,1.5);
            // Specular light component
            vec3 reflection = reflect(light_direction, normal);
            vec3 eyeDirection = normalize(cameraPosition - vertexPosition);
            light_specular += light_intensity * pow(max(0.1, dot(reflection,eyeDirection)), specularExponent) * light_colour/max(1,300/specularExponent)/2;

        }
    }

    vec3 light = light_ambient + light_diffuse + light_specular;

    if(isShaded)
    {
        out_Color = vec4(light, 1.0) * texturePattern;
    }
    else
    {
        out_Color = texturePattern;
    }
}
