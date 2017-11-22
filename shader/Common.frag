#version 430

in vec2 UV;
in vec3 normalWorld;
in vec4 light;
out vec4 daColor;

uniform sampler2D myTextureSampler;

uniform samplerCube cubeMap;


void main()
{
	vec3 Material_Clr = texture( myTextureSampler, UV).rgb;
	daColor= light * vec4(Material_Clr, 1.0);
	
	
}
