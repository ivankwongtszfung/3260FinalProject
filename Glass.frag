#version 430

in vec2 UV;
in vec4 light;
out vec4 daColor;

uniform sampler2D myTextureSampler;
uniform sampler2D myTextureSampler_1;
uniform samplerCube cubeMap;

void main()
{
	vec3 Material_Clr = (0.3*texture(myTextureSampler, UV) + 0.7*texture(myTextureSampler_1, UV)).rgb;
	

	daColor = light*vec4(Material_Clr, 1.0);
}
