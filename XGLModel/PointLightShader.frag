#version 330
const unsigned int MAX_LIGHT_NUM = 8u;
struct TagAttenuation{
	float Constant;
	float Linear;
	float Exp;
};
struct TagPointLight{
	vec3 Color;
	float AmbientIntensity;
	float DiffuseIntensity;
	vec3 Eposition;
	TagAttenuation Attenuation;
};

in vec3 epos;
in vec2 ftexcoord;
in vec3 enormal;

uniform unsigned int g_N;
uniform TagPointLight g_pointlight[MAX_LIGHT_NUM];
uniform float g_shineness;
uniform float g_materialIntensity;

uniform sampler2D g_sampler_pos;
uniform sampler2D g_sampler_diffuse;
uniform sampler2D g_sampler_normal;

out vec4 fcolor;

vec3 calcPointLight(in vec3 N, in vec3 E, in vec3 L, in float LD, in vec3 R, in TagPointLight light)
{
	vec3 lightColor = light.Color  * light.AmbientIntensity;
	float diffusef = dot(N, -L);
	if(diffusef > 0.0)
	{
		lightColor = lightColor + light.Color * light.DiffuseIntensity * diffusef;
	}

	float specularf = dot(R,-E);
	if(specularf > 0.0)
	{
		specularf = pow(specularf, g_shineness);
		lightColor = lightColor + light.Color * specularf * g_materialIntensity;
	}
	
	float attenuation = light.Attenuation.Constant + 
												light.Attenuation.Linear * LD +
													light.Attenuation.Exp * LD* LD;

	return lightColor / attenuation;
}

void main(){

	vec3 N = normalize(enormal);
	vec3 E =	normalize(epos);

	vec3 lightColor = vec3(0);

	for( unsigned int i = 0u; i < g_N; i++)
	{
		vec3 L = epos - g_pointlight[i].Eposition ;
		float LD = length(L);

		L = normalize(L);

		vec3 R = reflect(L, N);
		lightColor += calcPointLight(N,E,L,LD,R, g_pointlight[i]);
	}
	fcolor = vec4(lightColor,1.0);
}