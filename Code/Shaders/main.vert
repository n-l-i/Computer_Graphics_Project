#version 150

in  vec3 in_Position;
in  vec3 in_Normal;
in  vec2 in_TexCoord;

out vec3 normal;
out vec3 vertexPosition;
out vec2 texCoord;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 cameraMatrix;
uniform float time;

void main(void)
{
	gl_Position = projectionMatrix*cameraMatrix*modelMatrix*vec4(in_Position, 1.0);
	
	mat3 normalMatrix = mat3(modelMatrix);
	normal = normalize(normalMatrix*in_Normal);
	
	vertexPosition = vec3(modelMatrix*vec4(in_Position, 1.0));
	
	texCoord = in_TexCoord;
}
