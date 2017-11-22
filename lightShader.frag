#version 430

in vec2 UV;
in vec3 normalWorld;

out vec4 daColor;

uniform sampler2D myTextureSampler;


uniform vec3 eyePositionWorld;
uniform vec3 lightPositionWorld;
uniform vec3 ambientLight;
uniform vec3 coefficient_d;
uniform vec3 coefficient_s;
uniform vec4 lightColor;
void main()
{
	// Diffuse
	vec3 lightVectorWorld= lightPositionWorld;
	//brightness
	vec3 diffuseLight= coefficient_d*lightVectorWorld;

	// Specular
	vec3 reflectedLightVectorWorld= reflect(-lightVectorWorld, normalWorld);
	vec3 eyeVectorWorld= normalize(eyePositionWorld);
	float s = clamp(dot(reflectedLightVectorWorld, eyeVectorWorld), 0, 1);
	//color of spot light 
	vec3 specularLight= coefficient_s * s;
	daColor= vec4((ambientLight+ clamp(diffuseLight, 0, 1) + specularLight),1.0)* lightColor;
}