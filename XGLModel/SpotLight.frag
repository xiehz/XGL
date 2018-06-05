#version 330

in vec3 tn;
in vec2 tc0;
in vec3 eyep;


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

uniform sampler2D sampler2d;
uniform float specularIntensity;
uniform float shineness;
uniform tagSpotLight spotlight;

out vec4 clr;

void main(){
	vec4 tex = texture2D(sampler2d, tc0);

	vec3 N = normalize(tn);
	vec3 E = -normalize(eyep);
	vec3 L = eyep - spotlight.Eposition;
	float D = length(L);
	L = normalize(L);

	float spotf = dot(L, spotlight.Direction);
	if(spotf >= spotlight.Cutoff)
	{
		 vec3 R = reflect(L, N);

		vec4 lightColor = vec4(spotlight.Light.Color  * spotlight.Light.AmbientIntensity,1.0);
		float diffusef = dot(N, -L);
		if(diffusef > 0.0)
		{
		lightColor = lightColor + vec4(spotlight.Light.Color * spotlight.Light.DiffuseIntensity * diffusef, 1.0);
		}

		float specularf = dot(R,E);
		if(specularf > 0.0)
		{
		specularf = pow(specularf, shineness);
		lightColor = lightColor + vec4(spotlight.Light.Color * specularf,1.0);
		}
	
		float attenuation = spotlight.Attenuation.Constant + 
												spotlight.Attenuation.Linear * D +
													spotlight.Attenuation.Exp * D* D;
		
		lightColor = lightColor /attenuation;

		float decrease = (spotf - spotlight.Cutoff) / (1.0 - spotlight.Cutoff);
		clr =tex * lightColor * decrease;
	}
	else{
		clr = vec4(0.0,0.0,0.0,1.0);
	}

}