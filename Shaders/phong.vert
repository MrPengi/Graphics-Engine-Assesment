#version 450 // Phong Vert shader

layout(location = 0) in vec3 local_position;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec2 texture_coordinates;
layout(location = 3) in vec4 Tangent;

uniform mat4 projection_view_matrix;
uniform mat4 model_matrix;


out vec2 final_texture_coodinates;
out vec3 vNormal;
out vec3 vTangent;
out vec3 vBiTangent;
out vec4 vPosition;

uniform mat3 NormalMatrix;

void main()
{
	final_texture_coodinates = texture_coordinates;
	vPosition = model_matrix * vec4(local_position, 1);
    vNormal = NormalMatrix *  vec3(normal.x, normal.y, normal.z);
	vTangent = NormalMatrix * vec3(Tangent.x, Tangent.y, Tangent.z);
	vBiTangent = cross(vNormal, vTangent) * Tangent.w;
    gl_Position = projection_view_matrix * vec4(local_position, 1.0);
}