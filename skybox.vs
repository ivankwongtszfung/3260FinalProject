#version 430

in layout(location = 0) vec3 vertexPosition_modelspace;

uniform mat4 PM;
uniform mat4 MM;
uniform mat4 VM;

out vec3 UV;

void main()
{

    vec4 v = vec4(vertexPosition_modelspace, 1);
	gl_Position = PM * VM * MM * v;
	UV = vertexPosition_modelspace;

}