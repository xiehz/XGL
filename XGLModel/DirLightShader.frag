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

uniform vec2 g_screensize;
uniform sampler2D g_sampler_pos;
uniform sampler2D g_sampler_diffuse;
uniform sampler2D g_sampler_normal;

out vec4 fcolor;

vec3 calcDirectionLight(in vec3 N, in vec3 E, in vec3 L, in vec3 R)
{
	vec3 lightColor = g_directionlight.Color  * g_directionlight.AmbientIntensity;
	float diffusef = dot(N, -L);
	if(diffusef > 0.0)
	{
		lightColor = lightColor + g_directionlight.Color * g_directionlight.DiffuseIntensity * diffusef;
	}

	float specularf = dot(R,-E);
	if(specularf > 0.0)
	{
		specularf = pow(specularf, g_shineness);
		lightColor = lightColor + g_directionlight.Color * specularf * g_materialIntensity;
	}
	
	return lightColor;
}


vec2 getTexCoord()
{
    return gl_FragCoord.xy / g_screensize;
}

void main(){
	
	vec2 texcoord = getTexCoord();

	vec3 N = normalize(texture(g_sampler_normal, texcoord).xyz);
	vec3 L =normalize( g_directionlight.Direction);
	vec3 R  = reflect(L,N);
	vec3 E = normalize(texture(g_sampler_pos, texcoord).xyz);
	vec3 diffuse = texture(g_sampler_diffuse, texcoord).xyz;

	fcolor =vec4( diffuse * calcDirectionLight(N,E,L,R),1.0);


}