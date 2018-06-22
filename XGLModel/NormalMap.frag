#version 330

in vec2 tc0;
in vec3 tvn;
in vec3 eyep;
in vec3 etangent;
in vec3 ebtangent;

vec4 calcSpotLight(in vec3 tn, in vec3 eyep);
vec3 calcTBNNormal(in vec2 texcoord, in vec3 tvn, in vec3 etangent,in vec3 ebtangent);

uniform sampler2D g_sampler2d;
uniform sampler2D g_NormalMap;

out vec4 color ;

void main()
{
	vec3 tn = calcTBNNormal(tc0, tvn,etangent, ebtangent);
	vec4 light = calcSpotLight(tn,eyep ) ;
	color =light *  texture2D(g_sampler2d,tc0);
}

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

vec3 calcTBNNormal(in vec2 texcoord, in vec3 tvn, in vec3 etangent,in vec3 ebtangent)
{
	vec3 T = normalize(etangent);
	//vec3 B = normalize(ebtangent);
	vec3 N = normalize(tvn);
    T = normalize(T - dot(T, N) * N);

    vec3 B = cross(N, T);

	mat3 TBN = mat3(T,B,N);

	vec3 normal = texture(g_NormalMap, texcoord).xyz;
	normal = normal * 2.0 - vec3(1.0,1.0,1.0);

	normal = normalize(TBN * normal);
	
	return normal;
}


vec4 calcSpotLight(in vec3 tn, in vec3 eyep)
{

	vec4 clr = vec4(0.0, 0.0, 0.0, 1.0);
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
		specularf = pow(specularf, g_shineness);
		lightColor = lightColor + vec4(spotlight.Light.Color * specularf * g_specularIntensity,1.0);
		}
	
		float attenuation = spotlight.Attenuation.Constant + 
												spotlight.Attenuation.Linear * D +
													spotlight.Attenuation.Exp * D* D;
		
		lightColor = lightColor /attenuation;

		float decrease = (spotf - spotlight.Cutoff) / (1.0 - spotlight.Cutoff);
		clr = lightColor * decrease;
	}
	else{
		clr = vec4(0.0,0.0,0.0,1.0);
	}

	return clr;
}