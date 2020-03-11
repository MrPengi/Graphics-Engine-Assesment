#version 450 // Frag Shader

//THIS IS AN OUDATED SHADER
//GO TO PHONG.FRAG FOR THE MOST UP TO DATE SHADER THIS PROGRAM USES

//this was an updated frag shader after messing around before phong lighting was implemented
uniform sampler2D diffuse_texture;

in vec2 final_texture_coodinates;
out vec4 final_color;

void main()
{
    final_color = texture(diffuse_texture, final_texture_coodinates);
}