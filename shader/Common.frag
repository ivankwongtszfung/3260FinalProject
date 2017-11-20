#version 430

in vec2 UV;
in vec3 normalWorld;
in vec3 position;
out vec4 daColor;

uniform sampler2D myTextureSampler;

uniform samplerCube cubeMap;
uniform vec3 eyePositionWorld;
uniform vec3 lightPositionWorld;
uniform vec3 ambientLight;
uniform vec3 coefficient_d;
uniform vec3 coefficient_s;
uniform vec4 lightColor;

void main()
{
	vec3 position = vec3(0.0f, 0.0f, 0.0f);
	vec3 Material_Clr = texture( myTextureSampler, UV).rgb;

	// Diffuse
	vec3 lightVectorWorld= normalize(lightPositionWorld - position);
	float brightness = dot(lightVectorWorld, normalize(normalWorld));
	//brightness
	vec3 diffuseLight= coefficient_d*vec3(brightness);
	// Specular
	vec3 reflectedLightVectorWorld= reflect(-lightVectorWorld, normalWorld);
	vec3 eyeVectorWorld= normalize(eyePositionWorld-position);
	float s = clamp(dot(reflectedLightVectorWorld, eyeVectorWorld), 0, 1);
	//color of spot light 
	vec3 specularLight= coefficient_s * s;
	daColor= vec4((ambientLight+ clamp(diffuseLight, 0, 1) + specularLight),1.0)* vec4(Material_Clr, 1.0);
}
