#version 430

in vec3 UV;

out vec4 daColor;

uniform samplerCube myTextureSampler;

void main()
{	

	daColor = texture( myTextureSampler, UV);

}
