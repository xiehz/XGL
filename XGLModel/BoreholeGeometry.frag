#version 330

out vec4 color;
in vec2 texcoord;

uniform sampler2D g_sampler;

void main(){
	color = texture(g_sampler,texcoord );
}

vec4 valueToHSV(float v){

	const float HUE = 360.0f;
	const float SIX = 60.0f;

	float H = v * HUE ;
	const float S = 1;
	const float V = 1;

	float C = V* S;
	float X = C*(1 - abs(mod(H/ SIX,2) -1));
	float M = V -C;


	vec3 rgb;
	if(H>=0 && H< SIX)
	{
		rgb = vec3(C,X,0); 
	}
	else if(H>=SIX && H< SIX *2)
	{
			rgb = vec3(X,C,0);
	}
	else if(H>=SIX* 2 && H< SIX *3)
	{
			rgb = vec3(0,C,X);
	}
	else if(H>=SIX* 3 && H< SIX *4)
	{
			rgb = vec3(0,X,C);
	}
	else if(H>=SIX* 4 && H< SIX *5)
	{
			rgb = vec3(X, 0, C);
	}
	else if(H>=SIX* 5 && H< SIX *6)
	{
			rgb = vec3(C, 0, X);
	}
	else{
			rgb = vec3(0, 0,0);
	}

	return vec4( rgb.r + M, rgb.g + M , rgb.b + M , 1.0);
}
