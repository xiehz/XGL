#version 330
uniform sampler2D sampler2d;  

in vec2 texcoord0;
in vec3 normal0;
out vec4 color;

struct DLight{
	float AmbientDensity;
	float DiffuseDensity;

	vec3 Ambient;
	vec3 Diffuse;
	vec3 DiffuseDirection;
};

uniform DLight dLight;

void main()
{
	vec4 tcolor = texture2D(sampler2d, texcoord0.st);

	vec4 ambientColor = vec4(dLight.Ambient * dLight.AmbientDensity, 1.0);
	vec4 diffuseColor;
	float diffuseFactor = dot(normalize(normal0), normalize(-dLight.DiffuseDirection));
	if(diffuseFactor < 0.0)
	{
		diffuseColor = vec4(0.0,0.0,0.0,1.0);
	} 
	else{
		diffuseColor = vec4(dLight.Diffuse * dLight.DiffuseDensity * diffuseFactor, 1.0);
	}

	color = tcolor* (ambientColor + diffuseColor);
	//color = vec4(normalize(normal0),1.0);
}