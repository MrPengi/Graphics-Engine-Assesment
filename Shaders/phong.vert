#version 450 // Phong Vert shader

layout(location = 0) in vec3 local_position;
layout(location = 1) in vec4 normal;

uniform mat4 projection_view_matrix;
//uniform mat4 model_matrix;

out vec3 vNormal;

uniform mat3 NormalMatrix;

void main()
{
	//vec3 newModel = vec3(normal.x, normal.y, normal.z);
    vNormal = NormalMatrix * normal.xyz;
    gl_Position = projection_view_matrix * vec4(local_position, 1.0);
}