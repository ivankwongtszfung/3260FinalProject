#version 430

in vec2 UV;
in vec3 normalWorld;
in vec4 light;
in float visibility;
out vec4 daColor;

const vec3 fogColor = vec3(0.5, 0.5,0.5);

uniform sampler2D myTextureSampler;
uniform sampler2D myTextureSampler_1;
uniform samplerCube cubeMap;

void main()
{
	vec3 Material_Clr = (0.3*texture(myTextureSampler, UV) + 0.7*texture(myTextureSampler_1, UV)).rgb;

	vec4 lightColor = light  * vec4(Material_Clr, 1.0);
	daColor= vec4(mix(fogColor,lightColor.rgb,visibility),1);
}
