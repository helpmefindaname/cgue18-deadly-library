#version 330

uniform sampler2D colorBuffer;
uniform sampler2D positionBuffer;
uniform sampler2D normalBuffer;
uniform sampler2D materialBuffer;

uniform vec3 camera_world;

uniform mat4 modelMatrix;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform float lightIntensity;
uniform float lightRadius;

uniform float attenuationConstant;
uniform float attenuationLinear;
uniform float attenuationSquared;

uniform vec2 resolution;

out vec4 outputColor;


float calculcateLightPower(vec3 position, vec3 normal, vec4 material) {
	float diffuseIntensity = material.y;
	float specularIntensity = material.z;
	float attenuation = 0;
	specularIntensity = 0.0f;
	float specularPower = 32.0f;
	float specularFactor = 0.0f;
	float diffuseFactor = 0.0f;

	vec3 lightToVertex = position - lightPosition;
	float distance = length(lightToVertex);

	if (distance < lightRadius) {
		vec3 lightDirection = lightToVertex / distance;

		diffuseFactor = dot(normal, -lightDirection);
		if (diffuseFactor > 0) {

			attenuation = 1.0 / (attenuationConstant + attenuationLinear * distance + attenuationSquared * distance * distance);

			vec3 vertexToCamera = normalize(camera_world - position);
			vec3 vertexReflect = normalize(reflect(lightDirection, normal));

			specularFactor = dot(vertexToCamera, vertexReflect);
			if (specularFactor > 0) {
				specularFactor = pow(specularFactor, specularPower);
			}else{
			    specularFactor = 0.0f;
			}
		}else{
		   diffuseFactor = 0.0f;
		}
	}

	return (diffuseIntensity * diffuseFactor + specularIntensity * specularFactor) * lightIntensity * attenuation;
}

void main() {
	
	outputColor = vec4(.1f, .1f, .1f, 1.0f);
	vec2 bufferCoordinates = gl_FragCoord.xy / resolution;

	vec3 color = texture(colorBuffer, bufferCoordinates).xyz;
	vec3 position = texture(positionBuffer, bufferCoordinates).xyz;
	vec3 normal = normalize(texture(normalBuffer, bufferCoordinates).xyz);
	vec4 material = texture(materialBuffer, bufferCoordinates);

	float lightPower = calculcateLightPower(position, normal, material);

	outputColor = vec4(color * (lightColor * lightPower), 1.0);
}