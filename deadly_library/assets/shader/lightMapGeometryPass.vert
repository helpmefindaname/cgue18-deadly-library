#version 330

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexUV;

uniform mat4 modelMatrix;
uniform mat4 viewProjectionMatrix;

out vec3 fragPositionWorldspace;
out vec3 fragNormalWorldspace;
out vec2 fragUV;

void main() {
	vec4 worldspacePosition = modelMatrix * vec4(vertexPosition, 1.0);

	fragPositionWorldspace = worldspacePosition.xyz;
	fragNormalWorldspace = (modelMatrix * vec4(vertexNormal, 0.0)).xyz;
	fragUV = vertexUV;

	gl_Position = viewProjectionMatrix * worldspacePosition;
}