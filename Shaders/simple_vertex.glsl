#version 450
//THIS IS AN OUDATED SHADER
//GO TO PHONG.VERT FOR THE MOST UP TO DATE SHADER THIS PROGRAM USES

//This was the first vertex shader test
layout(location = 0) in vec3 local_position;

uniform mat4 projection_view_matrix;
uniform mat4 model_matrix; 

out vec4 vertexColor;

void main()
{
	gl_Position = (projection_view_matrix * model_matrix) * vec4(local_position, 1);
	vertexColor =  vec4 (gl_Position * 0.1f);
}