#version 150

in  vec3 normal;
in vec3 vertexPosition;
in  vec2 texCoord;
out vec4 out_Color;

uniform vec3 cameraPosition;

uniform sampler2D texUnit;
uniform mat4 time;
uniform float textureRepeats;
uniform bool isShaded;

uniform vec3 lightSourcesDirPosArr[4];
uniform vec3 lightSourcesColorArr[4];
uniform float specularExponent;
uniform bool isDirectional[4];

// ambient light
vec3 ambientLight(vec3 lightColour, float lightIntensity)
{
    lightColour = normalize(lightColour)*3;
    return lightColour*lightIntensity;
}

// diffuse phong light
vec3 diffuseLight(vec3 lightColour, float lightIntensity, vec3 lightPosition, vec3 vertexPosition, vec3 normal)
{
    lightColour = normalize(lightColour)*3;
    vec3 lightDirection = normalize(lightPosition - vertexPosition);
    float lightStrength = max(dot(normal, lightDirection), 0.0);
    return lightStrength*lightColour*lightIntensity;
}

// specular phong light
vec3 specularLight(vec3 lightColour, float lightIntensity, vec3 lightPosition, vec3 vertexPosition, vec3 normal, vec3 cameraPosition, float exponent)
{
    lightColour = normalize(lightColour)*3;
    vec3 lightDirection = normalize(lightPosition - vertexPosition);
    vec3 reflectedLightDirection = reflect(-lightDirection, normal);
    vec3 eyeDirection = normalize(cameraPosition - vertexPosition);
    float lightStrength = 0.0;
    if (dot(lightDirection, normal) > 0.0)
    {
        lightStrength = max(dot(reflectedLightDirection, eyeDirection), 0.01);
        lightStrength = pow(lightStrength, exponent);
    }
    return lightStrength*lightColour*lightIntensity;
}

// diffuse directional light
vec3 directionalLight(vec3 lightColour, vec3 lightDirection, vec3 normal, float lightIntensity)
{
    lightColour = normalize(lightColour)*3;
    float lightStrength = max(dot(normal, lightDirection), 0.0);
    return lightStrength*lightColour*lightIntensity;
}

void main(void)
{

    vec3 light = vec3(0.0, 0.0, 0.0);

    float lightIntensity = 1.0;
    int n = lightSourcesColorArr.length();
    for(int i = 0; i < n; i++)
    {
        vec3 lightColour = lightSourcesColorArr[i];

        if(isDirectional[i])
        {
            vec3 lightDirection = lightSourcesDirPosArr[i];
            vec3 directionalLight = directionalLight(lightColour, lightDirection, normal, lightIntensity);
            light += directionalLight / n;
        }
        else
        {
            vec3 lightPosition = lightSourcesDirPosArr[i];
            vec3 diffuseLight = diffuseLight(lightColour, lightIntensity, lightPosition, vertexPosition, normal);
            vec3 specularLight = specularLight(lightColour, lightIntensity, lightPosition, vertexPosition, normal, cameraPosition, specularExponent);
            light += (diffuseLight + specularLight) / n;
        }
    }

    vec4 texturePattern = texture(texUnit, texCoord * textureRepeats);

    if(isShaded)
    {
        out_Color = vec4(light, 1.0) * texturePattern;
    }
    else
    {
        out_Color = texturePattern;
    }
}
