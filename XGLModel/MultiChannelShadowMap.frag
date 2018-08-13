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


vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);

float calcShadowFactor(vec3 l)
{
	float shadow = 0.0;
	float bias = 0.15;
	int samples = 20;
	float viewDistance = length(l);
	float diskRadius = 0.05;
	for(int i = 0; i < samples; ++i)
	{
		float closestDepth = texture(g_shadowmap, l + sampleOffsetDirections[i] * diskRadius).r;
		if(viewDistance - EPSILON > closestDepth)
			shadow += 1.0;
	}
	shadow /= float(samples);
	return shadow * 0.8;
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
		lightColor += (calcPointLight(N,E,L,LD,R, g_pointlight[i]) * (1-factor)) ;
	}
	fcolor = vec4(lightColor,1.0) * texture(g_sampler2d, otex);
}