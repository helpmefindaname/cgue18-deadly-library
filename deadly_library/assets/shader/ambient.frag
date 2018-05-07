#version 430 core

uniform sampler2D colorBuffer;
uniform sampler2D materialBuffer;

uniform vec2 resolution;
uniform float brightness;

out vec4 outputColor;

void main() {
	vec2 bufferCoordinates = gl_FragCoord.xy / resolution;

	vec3 color = texture(colorBuffer, bufferCoordinates).xyz;
	vec4 material = texture(materialBuffer, bufferCoordinates);

	outputColor = vec4(color * (material.x * brightness), 1.0);
}