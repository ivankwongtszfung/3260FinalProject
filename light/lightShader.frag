#version 430

in vec4 light;

out vec4 daColor;

uniform sampler2D myTextureSampler;

void main()
{
	daColor= light;
}