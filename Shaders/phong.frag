#version 450 // Phong Frag Shader

uniform sampler2D diffuse_texture;
uniform sampler2D specular_texture;
uniform sampler2D normal_texture;

in vec2 final_texture_coodinates;
in vec3 vNormal;
in vec3 vTangent;
in vec3 vBiTangent;
in vec4 vPosition;


uniform vec3 Ia; //light ambient
uniform vec3 Id; //light diffuse
uniform vec3 Is; //light specular

uniform vec3 LightDirection;
uniform vec3 CameraColor;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;


uniform vec3 cameraPosition;

out vec4 final_color;

void main()
{
	float specularPower = 30.0f;
	
	vec3 N = normalize(vNormal);
	vec3 T = normalize(vTangent);
	vec3 B = normalize(vBiTangent);
	vec3 L = normalize(LightDirection);
	
	float lambertTerm = max ( 0, min( 1, dot ( N, -L ) ) );
	
	mat3 TBN = mat3(T,B,N);
	
	vec3 V = normalize(cameraPosition - vPosition.xyz);
	vec3 R = reflect(L, N);
	
	float specularTerm = pow(max(0, dot(R, V) ), specularPower);
	
	vec3 TexDiffuse = texture(diffuse_texture, final_texture_coodinates).rgb;
	vec3 TexSpecular = texture(specular_texture, final_texture_coodinates).rgb;
	vec3 TexNormal = texture(normal_texture, final_texture_coodinates).rgb;
	
	
	
	//vec3 ambient = Ia * Ka;
	//vec3 diffuse = Id * Kd * lambertTerm;
	//vec3 specular = Is * Ks * specularTerm;
	
	N = TBN * (TexNormal * 2 - 1);
	
	TexNormal = TexNormal * Ia * Ka * CameraColor;
	TexDiffuse = TexDiffuse * Id * Kd * lambertTerm;
	TexSpecular = TexSpecular * Is * Ks * specularTerm;
	
	
	final_color = vec4(TexDiffuse + TexSpecular + TexNormal, 1);
	
	//final_color = vec4(lambertTerm,lambertTerm,lambertTerm,1);
	//final_color = texture(diffuse_texture, final_texture_coodinates);
	//final_color = final_color * vec4(LightDirection, 1);
}