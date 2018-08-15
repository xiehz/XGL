#version 460 core

in vec2 texcoord;

uniform sampler2D g_posmap;
uniform float g_samplerrad;

uniform mat4 g_pers;

const int C_MAX_KERNEL_SIZE = 64;

uniform vec3 g_kernels[C_MAX_KERNEL_SIZE];

layout(location = 0) out float fragao;

void main(){

	vec3 pos = texture(g_posmap, texcoord).xyz;
	float AO = 0.0;
	float pass = 0;
	for(int i = 0; i< C_MAX_KERNEL_SIZE; i++){
		
		vec4 samplerPos = vec4(pos + g_kernels[i], 1.0);

		samplerPos = g_pers * samplerPos;
		samplerPos.xy /= samplerPos.w;

		vec2 samplertexcoord = (samplerPos.xy + vec2(1))/2.0;

		float samplerdepth = texture(g_posmap, samplertexcoord).z;
		if(abs(pos.z - samplerdepth) < g_samplerrad)
		{
			AO += step(samplerdepth, samplerPos.z);	
			pass += 1;
		}

	}
	
	AO = 1.0 - AO /C_MAX_KERNEL_SIZE;

	fragao = pow(AO,2.0) ;

//	fragao =1.0 - pass/C_MAX_KERNEL_SIZE;
//	fragao = AO /C_MAX_KERNEL_SIZE;
} 