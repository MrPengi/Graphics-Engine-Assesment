#version 450

uniform vec4 color = vec4(1,1,1,1);

in vec4 vertexColor;

//uniform mat4 viewStuff = ViewTransform;

out vec4 final_color;

void main()
{

	//final_color = vec4(color.x * vertexColor.x * 0.2f, color.x * vertexColor.y * 0.2f, color.x * vertexColor.z * 0.2f, 1);
	final_color = color;
}