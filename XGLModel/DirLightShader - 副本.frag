#version 330

struct TagDirectionLight{
	vec3 Color;
	float AmbientIntensity;
	float DiffuseIntensity;
	vec3 Direction;
};

uniform TagDirectionLight g_directionlight;
uniform float g_shineness;
uniform float g_materialIntensity;

uniform sampler2D g_sampler_pos;
uniform sampler2D g_sampler_diffuse;
uniform sampler2D g_sampler_normal;

in vec3 epos;
in vec2 ftexcoord;
in vec3 enormal;

out vec4 fcolor;

vec4 calcDirectionLight(in vec3 N, in vec3 E, in vec3 L, in vec3 R)
{
	vec4 lightColor = vec4(g_directionlight.Color  * g_directionlight.AmbientIntensity,1.0);
	float diffusef = dot(N, -L);
	if(diffusef > 0.0)
	{
		lightColor = lightColor + vec4(g_directionlight.Color * g_directionlight.DiffuseIntensity * diffusef, 1.0);
	}

	float specularf = dot(R,-E);
	if(specularf > 0.0)
	{
		specularf = pow(specularf, g_shineness);
		lightColor = lightColor + vec4(g_directionlight.Color * specularf * g_materialIntensity,1.0);
	}
	
	return lightColor;
}



void main(){
	
	vec3 N = normalize(enormal);
	vec3 L =normalize( g_directionlight.Direction);
	vec3 R  = reflect(L,N);
	vec3 E = normalize(epos);

	fcolor = calcDirectionLight(N,E,L,R);

}