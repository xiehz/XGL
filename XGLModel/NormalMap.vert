#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tc;
layout(location = 2) in vec3 n;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 btangent;

uniform mat4 g_mv;
uniform mat4 g_pers;


out vec2 tc0;
out vec3 tvn;
out vec3 eyep;
out vec3 etangent;
out vec3 ebtangent;

void main(){
	
	gl_Position = g_pers * g_mv * vec4(pos,1.0);
	mat3 transmv = mat3(transpose(inverse(g_mv)));
	etangent = transmv * tangent;
	ebtangent = transmv * btangent;
	tvn = transmv * n;

	tc0 = tc;
	eyep = (g_mv * vec4(pos,1.0)).xyz;
}

