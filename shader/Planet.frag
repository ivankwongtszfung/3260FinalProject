#version 430

in vec2 UV;
in vec3 normalWorld;
in vec4 light;
out vec4 daColor;

uniform sampler2D myTextureSampler;
uniform sampler2D myTextureSampler_1;

void main()
{

	vec3 normal = normalWorld;

	vec3 normalColor = normalize(texture( myTextureSampler_1, UV ).rgb*2.0 - 1.0);
	vec3 Material_Clr = texture(myTextureSampler, UV).rgb;
	daColor =light * vec4(Material_Clr,1.0);
	



}
