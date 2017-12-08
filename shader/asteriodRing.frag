#version 430

in vec2 UV;

out vec4 daColor;

const vec3 fogColor = vec3(0.5, 0.5,0.5);

uniform sampler2D myTextureSampler;



void main()
{
	vec3 Material_Clr = texture( myTextureSampler, UV).rgb;
	daColor= vec4(Material_Clr, 1.0);
	
}
