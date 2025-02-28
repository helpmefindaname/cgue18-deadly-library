#version 430 core

// Interpolated values from the vertex shaders
in vec2 uvs;

// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;

void main(){

	color = texture(myTextureSampler, uvs);
}