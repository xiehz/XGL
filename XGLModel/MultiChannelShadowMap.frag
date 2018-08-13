#version 460 core
#define EPSILON 0.00001

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

in vec3 worldn;
in vec2 otex;
in vec3 worldp;

uniform unsigned int g_N;
uniform TagPointLight g_pointlight[MAX_LIGHT_NUM];
uniform float g_shineness;
uniform float g_materialIntensity;
uniform vec3 g_eye_wordpos;

uniform samplerCube g_shadowmap;
uniform sampler2D g_sampler2d;
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




float calcShadowFactor(vec3 l)
{
    float sampledDistance = texture(g_shadowmap, l).r;

	return sampledDistance;
//    float distance = length(l);

//    if (distance <= sampledDistance + EPSILON)
//        return 1.0;
//    else
//        return 0.5;
}   

void main(){

	vec3 N = normalize(worldn);
	vec3 E =	normalize(worldp - g_eye_wordpos );

	vec3 lightColor = vec3(0);

	for( unsigned int i = 0u; i < g_N; i++)
	{
		vec3 L = worldp - g_pointlight[i].Eposition ;
		float factor = calcShadowFactor(L);

		float LD = length(L);

		L = normalize(L);

		vec3 R = reflect(L, N);
//		lightColor += (calcPointLight(N,E,L,LD,R, g_pointlight[i]) * factor);
		lightColor =vec3(factor,factor, factor);
//		lightColor +=L;

	}
	fcolor = vec4(lightColor,1.0) * texture(g_sampler2d, otex);
//	fcolor = vec4(worldn,1.0);
}