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

struct tagPointLight{
	vec3 Eposition;
	tagAttenuation Attenuation;
	tagBaseLight Light;
};

uniform sampler2D sampler2d;
uniform float specularIntensity;
uniform float shineness;
uniform tagPointLight pointLight;

out vec4 clr;

void main(){
	vec4 tex = texture2D(sampler2d, tc0);

	vec3 N = normalize(tn);
	vec3 E = -normalize(eyep);
	vec3 L = eyep - pointLight.Eposition;
	float D = length(L);
	L = normalize(L);
	vec3 R = reflect(L, N);

	vec4 lightColor = vec4(pointLight.Light.Color  * pointLight.Light.AmbientIntensity,1.0);
	float diffusef = dot(N, -L);
	if(diffusef > 0.0)
	{
		lightColor = lightColor + vec4(pointLight.Light.Color * pointLight.Light.DiffuseIntensity * diffusef, 1.0);
	}

	float specularf = dot(R,E);
	if(specularf > 0.0)
	{
		specularf = pow(specularf, shineness);
		lightColor = lightColor + vec4(pointLight.Light.Color * specularf,1.0);
	}
	
	float attenuation = pointLight.Attenuation.Constant + 
												pointLight.Attenuation.Linear * D +
													pointLight.Attenuation.Exp * D* D;
	clr =tex*  (lightColor / attenuation );
}