#version 430

in vec3 UV;
in float visibility;
out vec4 daColor;

uniform samplerCube myTextureSampler;

const vec3 fogColor = vec3(0.5, 0.5,0.5);

void main()
{
	vec3 material = texture( myTextureSampler, UV).rgb;
	vec4 lightColor = vec4(material,1.0f);
	//daColor = texture( myTextureSampler, UV);
	//vec4 lightColor = vec4(1.0f);
	daColor= vec4(mix(fogColor,lightColor.rgb,visibility),1);	



	

}
