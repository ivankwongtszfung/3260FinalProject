#version 430

in layout(location = 0) vec3 vertexPosition_modelspace;
in layout(location = 1) vec2 vertexUV;
in layout(location = 2) vec3 normal;
in layout (location = 3) mat4 instanceMatrix;

out vec2 UV;

uniform mat4 MM;
uniform mat4 PM;
uniform mat4 VM;


void main()
{
    gl_Position = MM * PM * VM * instanceMatrix * vec4(vertexPosition_modelspace, 1.0); 
    UV = vertexUV;
}



