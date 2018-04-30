#version 430 core

// Interpolated values from the vertex shaders
in vec2 uvs;

// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;

void main(){

	color = texture( myTextureSampler, uvs);

	if(color.a < 0.5f) 
	    color = vec4(0.0f,0.0f,0.0f,1.0f);

    color = vec4(uvs.x, 1.0f-uvs.x, 0.0f,1.0f);

	//color = vec4(1.0f,1.0f,1.0f,1.0f);
}