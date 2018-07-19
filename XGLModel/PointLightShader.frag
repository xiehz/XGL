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

uniform unsigned int g_N;
uniform TagPointLight g_pointlight[MAX_LIGHT_NUM];
uniform float g_shineness;
uniform float g_materialIntensity;

uniform vec2 g_screensize;
uniform sampler2D g_sampler_pos;
uniform sampler2D g_sampler_diffuse;
uniform sampler2D g_sampler_normal;

out vec4 fcolor;

vec3 calcPointLight(in vec3 N, in vec3 E, in vec3 L, in float LD, in vec3 R, in TagPointLight light)
{


		float attenuation = light.Attenuation.Constant + 
												light.Attenuation.Linear * LD +
													light.Attenuation.Exp * LD* LD;

	if(attenuation > 3)
		return vec3(0.0);

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

	return lightColor / attenuation  ;
}

vec2 getScreenTexCoord(){
	return gl_FragCoord.xy/ g_screensize;
}

void main(){

	vec2 texcoord = getScreenTexCoord();

	vec3 N = normalize(texture(g_sampler_normal, texcoord).xyz);
	vec3 EPOS = texture(g_sampler_pos, texcoord).xyz;
	vec3 E =	normalize(EPOS);

	vec3 lightColor = vec3(0);


	vec3 L = EPOS - g_pointlight[g_N].Eposition ;
	float LD = length(L);

	L = normalize(L);

	vec3 R = reflect(L, N);
	lightColor += calcPointLight(N,E,L,LD,R, g_pointlight[g_N]);
	

	vec3 diffuse = texture(g_sampler_diffuse, texcoord).xyz;
	fcolor = vec4(diffuse * lightColor,1.0);

}