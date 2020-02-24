#version 450 // Phong Frag Shader

//uniform sampler2D diffuse_texture;

in vec3 vNormal;

uniform vec3 Ia; //ambient light colour

uniform vec3 Id; //diffuse light colour

uniform vec3 Is; //specular light colour

uniform vec3 LightDirection;

//in vec2 final_texture_coodinates;
out vec4 final_color;

void main()
{
	vec3 N = normalize(vNormal);
	vec3 L = normalize(LightDirection);
	
	float lambertTerm = max ( 0, min( 1, dot ( N, -L ) ) );
	
	//vec3 diffuse = Id * lambertTerm;
	final_color = vec4(lambertTerm,lambertTerm,lambertTerm,1);
    //final_color = vec4 (diffuse, 1);
}