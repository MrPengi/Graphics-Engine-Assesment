#version 450 // Phong Frag Shader

uniform sampler2D diffuse_texture;
uniform sampler2D specular_texture;
uniform sampler2D normal_texture;

in vec2 final_texture_coodinates;
in vec3 vNormal;
in vec3 vTangent;
in vec3 vBiTangent;
in vec4 vPosition;


uniform vec3 Ia; //light colour

uniform vec3 LightDirection;


out vec4 final_color;

void main()
{
	vec3 N = normalize(vNormal);
	vec3 T = normalize(vTangent);
	vec3 B = normalize(vBiTangent);
	vec3 L = normalize(LightDirection);
	
	
	mat3 TBN = mat3(T,B,N);
	
	
	
	vec3 TexDiffuse = texture(diffuse_texture, final_texture_coodinates).rgb;
	vec3 TexSpecular = texture(specular_texture, final_texture_coodinates).rgb;
	vec3 TexNormal = texture(normal_texture, final_texture_coodinates).rgb;
	
	
	
	float lambertTerm = max ( 0, min( 1, dot ( N, -L ) ) );
	
	
	vec3 diffuse = Ia * lambertTerm;
	
	N = TBN * (TexNormal * 2 - 1);
	
	
	
	final_color = vec4(diffuse + TexNormal, 1);
	
	//final_color = vec4(lambertTerm,lambertTerm,lambertTerm,1);
	//final_color = texture(diffuse_texture, final_texture_coodinates);
	//final_color = final_color * vec4(LightDirection, 1);
}