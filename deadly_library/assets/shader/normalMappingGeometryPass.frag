#version 330

in vec3 fragPositionWorldspace;
in vec3 fragNormalWorldspace;
in vec2 fragUV;

uniform bool useTexture;
uniform sampler2D textureBuffer;
uniform sampler2D normalMapBuffer;
uniform sampler2D depthMapBuffer;
uniform mat4 modelMatrix;

uniform vec3 materialColor;
uniform float ambientFactor;
uniform float diffuseFactor;
uniform float specularFactor;

layout(location = 0) out vec3 color;
layout(location = 1) out vec3 position;
layout(location = 2) out vec3 normal;
layout(location = 3) out vec4 material;

void main() {
	if (useTexture) {
		color = texture(textureBuffer, fragUV).rgb;
	} else {
		color = materialColor;
	}

	vec3 vertexNormal = texture(normalMapBuffer, fragUV).rgb * 2.0 - 1.0;

	position = fragPositionWorldspace + fragNormalWorldspace * texture(depthMapBuffer, fragUV).x * 2.0;
	normal = normalize((modelMatrix * vec4(vertexNormal, 0.0)).xyz);
	material = vec4(ambientFactor, diffuseFactor, specularFactor, 0.0);
}