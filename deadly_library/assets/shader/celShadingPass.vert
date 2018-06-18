#version 330

in vec3 vertexPosition;
in vec2 vertexUV;

out vec2 uvs;

void main() {

	gl_Position =  vec4(vertexPosition.xy,0.0f,1.0f);

	uvs = vertexUV;
}
