#version 330

struct tagBaseLight{
	vec3 Color;
	float AmbientIntensity;
	float DiffuseIntensity;
};

struct tagAttenuation{
	float Constant;
	float Linear;
	float Exp;
};

struct tagSpotLight{
	vec3 Eposition;
	vec3 Direction;
	float Cutoff;
	tagAttenuation Attenuation;
	tagBaseLight Light;
};


uniform float g_specularIntensity;
uniform float g_shineness;
uniform tagSpotLight spotlight;
uniform sampler2D g_sampler2d;

in vec3 tnormal;
in vec2 otex;
in vec3 eyep;

out vec4 color ;

vec4 calcLight(in vec3 tn, in vec3 eyep);
void main()
{
	vec4 light = calcLight(tnormal,eyep ) ;
	color = light * texture2D(g_sampler2d, otex);	
}


vec4 calcLight(in vec3 tn, in vec3 eyep)
{
	vec3 N = normalize(tn);
	vec3 E = -normalize(eyep);
	vec3 L = eyep - spotlight.Eposition;
	float D = length(L);
	L = normalize(L);

	vec4 lightColor = vec4(0.0,0.0,0.0,1.0);

	float spotf = dot(L, spotlight.Direction);
	if(spotf >= spotlight.Cutoff)
	{
		 vec3 R = reflect(L, N);

		lightColor = vec4(spotlight.Light.Color  * spotlight.Light.AmbientIntensity,1.0);

		float diffusef = dot(N, -L);
		if(diffusef > 0.0)
		{
			lightColor += vec4(spotlight.Light.Color * spotlight.Light.DiffuseIntensity * diffusef, 1.0);
		}

		float specularf = dot(R,E);
		if(specularf > 0.0)
		{
			specularf = pow(specularf, g_shineness);
			lightColor += vec4(spotlight.Light.Color * specularf * g_specularIntensity,1.0);
		}

		float attenuation = spotlight.Attenuation.Constant + 
												spotlight.Attenuation.Linear * D +
													spotlight.Attenuation.Exp * D* D;
		
		lightColor = lightColor /attenuation;

		float dc = (spotf - spotlight.Cutoff) / (1.0 - spotlight.Cutoff);
		lightColor *= dc;
	}
	return lightColor;
}


