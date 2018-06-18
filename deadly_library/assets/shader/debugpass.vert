#version 330

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexUV;

uniform mat4 modelMatrix;
uniform mat4 viewProjectionMatrix;

void main() {
	gl_Position = viewProjectionMatrix * modelMatrix * vec4(vertexPosition, 1.0);
}