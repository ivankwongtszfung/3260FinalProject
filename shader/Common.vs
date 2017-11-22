#version 430

in layout(location = 0) vec3 vertexPosition_modelspace;
in layout(location = 1) vec2 vertexUV;
in layout(location = 2) vec3 normal;

uniform mat4 PM;
uniform mat4 MM;
uniform mat4 VM;

uniform vec3 eyePositionWorld;
uniform mat4 lightPosition;
uniform vec3 lightPositionWorld;

uniform vec3 ambientLight;
uniform vec3 coefficient_d;
uniform vec3 coefficient_s;
uniform vec4 lightColor;

out vec3 normalWorld;
out vec2 UV;
out vec4 light;

void main()
{
    vec4 v = vec4(vertexPosition_modelspace, 1.0);
    vec4 position =  PM * VM * MM * v;
	gl_Position = PM * VM * MM * v;
	vec4 testPos = MM * v;
	vec4 normal_temp = MM * vec4(normal, 0);
	normalWorld = normal_temp.xyz;

	UV = vertexUV;

	// Diffuse
	//vec3 lightPositionWorld=lightPosition.xyz;
	// Will be used for attenuation.
    float distance = length(lightPositionWorld - testPos.xyz);
	vec3 lightVectorWorld= normalize(lightPositionWorld - testPos.xyz);
	float brightness =  dot(lightVectorWorld, normalize(normalWorld));
	//brightness
	//diffuse = diffuse;
	vec3 diffuseLight= coefficient_d*vec3(brightness);
	// Specular
	vec3 reflectedLightVectorWorld= reflect(-lightVectorWorld, normalize(normalWorld));
	vec3 eyeVectorWorld= normalize(eyePositionWorld-testPos.xyz);
	float s = clamp(dot(reflectedLightVectorWorld, lightVectorWorld), 0, 1);
	//color of spot light 
	s=pow(s,50);
	vec3 specularLight= vec3(s);
	light=vec4((ambientLight+ clamp(diffuseLight, 0, 1) + specularLight),1.0);

	
}