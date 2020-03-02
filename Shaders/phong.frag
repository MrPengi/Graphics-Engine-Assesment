#version 450 // Phong Frag Shader

uniform sampler2D diffuse_texture;
uniform sampler2D specular_texture;
uniform sampler2D normal_texture;

in vec2 final_texture_coodinates;
in vec3 vNormal;
in vec3 vTangent;
in vec3 vBiTangent;
in vec4 vPosition;

//in vec4 vShadowCoord;

//uniform sampler2D shadowMap;

uniform vec3 Ia; //light ambient
uniform vec3 Id; //light diffuse
uniform vec3 Is; //light specular
uniform vec3 LightDirection;
uniform vec3 CameraColor;

uniform vec3 Ia2; //light ambient2
uniform vec3 Id2; //light diffuse2
uniform vec3 Is2; //light specular2


uniform vec3 LightDirection2;
uniform vec3 CameraColor2;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;


uniform vec3 cameraPosition;

//out float fragDepth;
out vec4 final_color;

void main()
{
	//power of the specular light
	float specularPower = 50.0f;
	
	//normalize variables
	vec3 N = normalize(vNormal);
	vec3 T = normalize(vTangent);
	vec3 B = normalize(vBiTangent);
	vec3 L = normalize(LightDirection);
	vec3 L2 = normalize(LightDirection2);
	
	//do calculation twice (once for both lights)
	float lambertTerm = max ( 0, min( 1, dot ( N, -L ) ) );
	
	float lambertTerm2 = max ( 0, min( 1, dot ( N, -L2 ) ) );
	
	//advanced texturing?
	mat3 TBN = mat3(T,B,N);
	
	vec3 V = normalize(cameraPosition - vPosition.xyz);
	vec3 R = reflect(L, N);
	
	vec3 R2 = reflect(L2, N);
	
	//once again calculate twice
	float specularTerm = pow(max(0, dot(R, V) ), specularPower);
	
	float specularTerm2 = pow(max(0, dot(R2, V) ), specularPower);
	
	vec3 TexDiffuse = texture(diffuse_texture, final_texture_coodinates).rgb;
	vec3 TexSpecular = texture(specular_texture, final_texture_coodinates).rgb;
	vec3 TexNormal = texture(normal_texture, final_texture_coodinates).rgb;
	
	//shadows?
	/*float d = max(0, dot(normalize(vNormal.xyz), LightDirection));
	float d2 = max(0, dot(normalize(vNormal.xyz), LightDirection2));
	
	if(texture(shadowMap, vShadowCoord.xy).r < vShadowCoord.z)
	{
		d = 0;
		
	}*/
	
	//advanced texturing?
	N = TBN * (TexNormal * 2 - 1);
	
	vec3 ambient = TexNormal * Ia  * Ka;
	vec3 diffuse = TexDiffuse * Id * Kd * lambertTerm;
	vec3 specular = TexSpecular * Is * Ks * specularTerm;
	

	vec3 ambient2 = TexNormal * Ka * Ia2;
	vec3 diffuse2 = TexDiffuse * Kd * Id2 * lambertTerm2;
	vec3 specular2 = TexSpecular * Ks * Is2 * specularTerm2;
	
	
	//final color = ambient + dissuse + specular from both light sources
	final_color = vec4(diffuse + specular + ambient + ambient2 + specular2 + diffuse2, 1);
	//fragDepth = gl_FragCoord.z;
	//final_color = vec4(lambertTerm,lambertTerm,lambertTerm,1);
	//final_color = texture(diffuse_texture, final_texture_coodinates);
	//final_color = final_color * vec4(LightDirection, 1);
}