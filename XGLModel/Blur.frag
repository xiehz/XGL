#version 460 core

in vec2 texcoord;
out float fragao;

//glsl数组是强类型
float offset[4] = float[](-1.5, -0.5, 0.5, 1.5); 

//两种写法
//float[] offset1 = float[](-1.5, -0.5, 0.5, 1.5);

uniform sampler2D g_aomap;


void main(){
	vec2 temp;
	fragao = 0;

	for(int i = 0; i< 4; ++i){
		for(int j = 0; j < 4; ++j){
			
			temp = texcoord;
			temp.x += offset[j]/ textureSize(g_aomap, 0).x;
			temp.y += offset[i]/ textureSize(g_aomap, 0).y;

			fragao += texture(g_aomap, temp).r;
		}
	}

	fragao /= 16.0;
}