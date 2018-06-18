#version 330

in vec3 fragPositionWorldspace;
in vec3 fragNormalWorldspace;
in vec2 fragUV;

uniform bool useTexture;
uniform bool useNormals;
uniform sampler2D textureBuffer;
uniform sampler2D normalMapBuffer;
uniform sampler2D depthMapBuffer;
uniform mat4 modelMatrix;

uniform vec3 materialColor;
uniform float ambientFactor;
uniform float diffuseFactor;
uniform float specularFactor;

uniform vec3[100] lightPositions;
uniform int lightCount;

uniform vec3 lightColor;
uniform float lightIntensity;
uniform float lightRadius;

uniform float attenuationConstant;
uniform float attenuationLinear;
uniform float attenuationSquared;

uniform float brightness;

out vec4 outputColor;


float calculcateLightPower(vec3 position, vec3 normal, vec3 lightPosition) {
	
	float attenuation = 0;
	float diffuseIntensity = 0.0f;

	vec3 lightToVertex = position - lightPosition;
	float distanceSqr = dot(lightToVertex, lightToVertex);

	if (distanceSqr < lightRadius * lightRadius) {
		float distance = sqrt(distanceSqr);
		vec3 lightDirection = lightToVertex / distance;

		diffuseIntensity = dot(normal, -lightDirection);
		if (diffuseIntensity > 0) {
			attenuation = 1.0 / (attenuationConstant + attenuationLinear * distance + attenuationSquared * distanceSqr);
		}else {
		   diffuseIntensity = 0.0f;
		}
	}

	return (diffuseIntensity * diffuseFactor) * lightIntensity * attenuation;
}

vec3 calculcateDiffuseLight(vec3 color, vec3 position, vec3 normal) {
    vec3 returnColor = vec3(0.0f);
	vec3 calcColor = color * lightColor;
	for(int i = 0; i < lightCount; i++){
		returnColor = returnColor + calcColor * calculcateLightPower(position, normal, lightPositions[i]);
	}
	return returnColor;
}

vec3 calculateAmbientLight(vec3 color) {
	return color * ambientFactor;
}

void main() {
	
	outputColor = vec4(.1f, .1f, .1f, 1.0f);
	
	vec2 bufferCoordinates = fragUV;

	vec3 color;
	if(useTexture) {
	   color=texture(textureBuffer, fragUV).rgb;
	}else{
	   color=materialColor;
	}
	vec3 position = fragPositionWorldspace;
	vec3 normal = fragNormalWorldspace;
	if(useNormals) {
		position = fragPositionWorldspace + fragNormalWorldspace * texture(depthMapBuffer, fragUV).x * 2.0;
		normal = normalize((modelMatrix * vec4(texture(normalMapBuffer, fragUV).rgb * 2.0 - 1.0, 0.0)).xyz);
	}

	vec3 ambientLight = calculateAmbientLight(color);
	vec3 light = calculcateDiffuseLight(color, position, normal);
	
	outputColor = vec4(brightness * (ambientLight + light), 1.0);
	/**/
}