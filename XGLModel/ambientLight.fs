#version 330

in vec2 texcoord0;
flat in vec4 color0;

uniform sampler2D sampler;

out vec4  oc;

struct tagAmbient{
	float density;
	vec3 ambient;
};

uniform tagAmbient ambient;

void main(){
	oc = texture2D(sampler, texcoord0.st);
	oc = oc * color0;
	//oc = clamp(oc, 0.0, 1.0);
}