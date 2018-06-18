#version 330

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUV;

uniform mat4 modelMatrix;

out vec3 fragPositionWorldspace;
out vec3 fragNormalWorldspace;
out vec2 fragUV;

void main() {
	vec4 worldspacePosition = modelMatrix * vec4(vertexPosition, 1.0);

	fragPositionWorldspace = worldspacePosition.xyz;
	fragNormalWorldspace = (modelMatrix * vec4(vertexNormal, 0.0)).xyz;
	fragUV = vertexUV;

	gl_Position =  vec4(vertexUV * 2 - 1,  0.0f, 1.0f);
}