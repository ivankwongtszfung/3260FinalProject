#version 430

in layout(location = 0) vec3 vertexPosition_modelspace;

uniform mat4 PM;
uniform mat4 MM;
uniform mat4 VM;
uniform float FogDensity;
uniform float FogGradient;
uniform bool FogFlag;

out float visibility;
out vec3 UV;

void main()
{

    vec4 v = vec4(vertexPosition_modelspace, 1);
	gl_Position = PM * VM * MM * v;
	UV = vertexPosition_modelspace;

	if(FogFlag)
	{
		float distance = clamp(length(VM * MM * v),0,10000000);
		visibility = exp(-pow((distance*FogDensity),FogGradient));
		visibility = clamp(visibility,0,1);
	}
	else{
		visibility = 1;
	}

}


