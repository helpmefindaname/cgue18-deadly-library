#version 330

uniform sampler2D inputBuffer;
uniform int celCount;

in vec2 uvs;

out vec4 outputColor;


void main() {
	outputColor = vec4(.1f, .1f, .1f, 1.0f);
	vec2 bufferCoordinates = uvs;
	vec3 color = texture(inputBuffer, bufferCoordinates).xyz;

	vec3 col =  floor(color * celCount);

	outputColor =vec4( col / (celCount - 1), 1.0);
}