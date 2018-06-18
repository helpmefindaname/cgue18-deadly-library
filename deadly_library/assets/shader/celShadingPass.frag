#version 330

uniform sampler2D inputBuffer;
uniform sampler2D materialBuffer;
uniform int celCount;

in vec2 uvs;

out vec4 outputColor;


void main() {
	outputColor = vec4(.1f, .1f, .1f, 1.0f);
	vec2 bufferCoordinates = uvs;
	vec3 color = texture(inputBuffer, bufferCoordinates).xyz;

	if(texture(materialBuffer, bufferCoordinates).z > 0.5f) {
		float l = length(color);
		color = (color/l) * floor(l * celCount) / (celCount - 1);
	}
	outputColor =vec4(color, 1.0);
}