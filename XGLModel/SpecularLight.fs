#version 330
in vec2 texcoord0;
in vec3 enormal;
in vec3 epos;
out vec4 color;

struct DLight                                                             
{                                                                                   
    vec3 Ambient;                                                                     
    float AmbientDensity;  
	vec3 Diffuse;                                                       
    float DiffuseDensity;                                                         
    vec3 DiffuseDirection;    
	vec3 Specular;                                                             
};    

uniform DLight dLight;
uniform sampler2D sampler2d;
uniform float gMaterialDensity;
uniform float gMaterialExp;

void main()
{
	vec4 texColor = texture2D(sampler2d, texcoord0.st);

	vec4 ambientColor = vec4(dLight.Ambient * dLight.AmbientDensity ,1.0);
	vec4 diffuseColor;
	vec3 normal = normalize(enormal);
	vec3 dir = -dLight.DiffuseDirection ;

	float diffuseFactor = dot(normal , dir);
	if(diffuseFactor < 0.0 )
	{
		diffuseColor = vec4(0.0,0.0,0.0,1.0);
	}
	else
	{
		diffuseColor = vec4(dLight.Diffuse* dLight.DiffuseDensity * diffuseFactor, 1.0);
	}


	vec4 specularColor ;

	vec3 rv = reflect(dLight.DiffuseDirection, normal);
	vec3 eye = -epos;
	float specularFactor = dot(rv, eye); 

	if(specularFactor < 0.0)
	{
		specularColor = vec4(0.0, 0.0, 0.0,1.0);
	}
	else
	{
		specularFactor = pow(specularFactor , gMaterialExp);
		specularColor = vec4(dLight.Specular * gMaterialDensity * specularFactor, 1.0);
	}

	color = texColor * (ambientColor + diffuseColor  + specularColor);

	color = vec4(1.0,1.0,0.0,1.0);
}