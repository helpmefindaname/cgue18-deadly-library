#version 330

uniform sampler2D colorBuffer;
uniform sampler2D positionBuffer;
uniform sampler2D normalBuffer;
uniform sampler2D materialBuffer;

uniform vec3 camera_world;

uniform vec3[100] lightPositions;
uniform int lightCount;

uniform vec3 lightColor;
uniform float lightIntensity;
uniform float lightRadius;

uniform float attenuationConstant;
uniform float attenuationLinear;
uniform float attenuationSquared;

uniform float brightness;

in vec2 uvs;

out vec4 outputColor;


float calculcateLightPower(vec3 position, vec3 normal, vec4 material, vec3 lightPosition) {
	float diffuseIntensity = material.y;
	float specularIntensity = material.z;
	float attenuation = 0;
	specularIntensity = 0.0f;
	float specularPower = 32.0f;
	float specularFactor = 0.0f;
	float diffuseFactor = 0.0f;

	vec3 lightToVertex = position - lightPosition;
	float distanceSqr = dot(lightToVertex, lightToVertex);

	if (distanceSqr < lightRadius * lightRadius) {
		float distance = sqrt(distanceSqr);
		vec3 lightDirection = lightToVertex / distance;

		diffuseFactor = dot(normal, -lightDirection);
		if (diffuseFactor > 0) {

			attenuation = 1.0 / (attenuationConstant + attenuationLinear * distance + attenuationSquared * distanceSqr);

			vec3 vertexToCamera = normalize(camera_world - position);
			vec3 vertexReflect = normalize(reflect(lightDirection, normal));

			specularFactor = dot(vertexToCamera, vertexReflect);
			if (specularFactor > 0) {
				specularFactor = pow(specularFactor, specularPower);
			}else{
			    specularFactor = 0.0f;
			}
		}else {
		   diffuseFactor = 0.0f;
		}
	}

	return (diffuseIntensity * diffuseFactor + specularIntensity * specularFactor) * lightIntensity * attenuation;
}

vec3 calculcateDiffuseLight(vec3 color, vec3 position, vec3 normal, vec4 material) {
    vec3 returnColor = vec3(0.0f);
	vec3 calcColor = color * lightColor;
	for(int i = 0; i < lightCount; i++){
		returnColor = returnColor + calcColor * calculcateLightPower(position, normal, material, lightPositions[i]);
	}
	return returnColor;
}

vec3 calculateAmbientLight(vec3 color, vec4 material) {
	return color * material.x;
}

void main() {
	
	outputColor = vec4(.1f, .1f, .1f, 1.0f);
	vec2 bufferCoordinates = uvs;

	vec3 color = texture(colorBuffer, bufferCoordinates).xyz;
	vec3 position = texture(positionBuffer, bufferCoordinates).xyz;
	vec3 normal = normalize(texture(normalBuffer, bufferCoordinates).xyz);
	vec4 material = texture(materialBuffer, bufferCoordinates);

	vec3 ambientLight = calculateAmbientLight(color, material);
	vec3 light = calculcateDiffuseLight(color, position, normal, material);

	outputColor = vec4(brightness * (ambientLight + light), 1.0);
}