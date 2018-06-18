#version 330

in vec3 fragPositionWorldspace;
in vec3 fragNormalWorldspace;
in vec2 fragUV;

uniform sampler2D lightMapBuffer;

uniform vec3 materialColor;
uniform float ambientFactor;
uniform float diffuseFactor;
uniform bool useCelShading;

layout(location = 0) out vec3 color;
layout(location = 1) out vec3 position;
layout(location = 2) out vec3 normal;
layout(location = 3) out vec4 material;

void main() {

	color = texture(lightMapBuffer, fragUV).rgb;
	
	position = fragPositionWorldspace;
	normal = normalize(fragNormalWorldspace);
	material = vec4(ambientFactor, diffuseFactor, useCelShading, 1.0);
}