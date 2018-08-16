#version 460 core

in vec2 texcoord;
in vec2 viewray;

uniform sampler2D g_posmap;
uniform float g_samplerrad;

uniform mat4 g_pers;

const int C_MAX_KERNEL_SIZE = 64;

uniform vec3 g_kernels[C_MAX_KERNEL_SIZE];

layout(location = 0) out float fragao;

float calcviewz(vec2 coords){
	
	float depth = texture(g_posmap, coords).x;

	return g_pers[3][2] /(1 -2 *depth- g_pers[2][2]);
}

float calcviewx(float ndcx, float viewz)
{
	return -(viewz* ndcx + g_pers[3][0])/ g_pers[0][0];
}

float calcviewy(float ndcy, float viewz)
{
	return -(viewz* ndcy + g_pers[3][1])/ g_pers[1][1];
}

void main(){

	float viewz = calcviewz(texcoord);
	float viewx = calcviewx(viewray.x ,viewz);
	float viewy = calcviewy(viewray.y , viewz);

	vec3 pos = vec3(viewx, viewy,viewz);
	float AO = 0.0;
	float pass = 0;
	for(int i = 0; i< C_MAX_KERNEL_SIZE; i++){
		
		vec4 samplerPos = vec4(pos + g_kernels[i], 1.0);

		samplerPos = g_pers * samplerPos;
		samplerPos.xy /= samplerPos.w;

		vec2 samplertexcoord = (samplerPos.xy + vec2(1))/2.0;

		float samplerdepth = calcviewz(samplertexcoord);
		if(abs(pos.z - samplerdepth) < g_samplerrad)
		{
			AO += step(samplerdepth, samplerPos.z);	
			pass += 1;
		}

	}
	
	AO = 1.0 - AO /C_MAX_KERNEL_SIZE;

	fragao = pow(AO,2.0) ;

//	fragao = viewx;
//	fragao = pos.x;
//	fragao =1.0 - pass/C_MAX_KERNEL_SIZE;
//	fragao = AO /C_MAX_KERNEL_SIZE;
} 