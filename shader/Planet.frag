#version 430

in vec2 UV;
in vec3 normalWorld;

out vec4 daColor;

uniform sampler2D myTextureSampler;
uniform sampler2D myTextureSampler_1;

void main()
{
	vec3 Material_Clr = texture(myTextureSampler, UV).rgb;
	daColor = vec4(Material_Clr, 1.0);

	vec3 normal = normalize(normalWorld);
	normal = texture(myTextureSampler_1,UV).rgb;
	normal = normalize(normal*2.0-1.0);

}