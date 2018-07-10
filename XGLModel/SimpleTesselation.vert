
#version 410 core


uniform mat4 g_world;

void main(){
	const vec4 vertices[] = vec4[](vec4(-1.0, -1.0, 0.0,1.0),
	vec4(1.0, -1.0,0.0, 1.0),
	vec4(1.0, 1.0, 0.0, 1.0),
		vec4(1.0, 1.0, 0.0, 1.0),
		vec4(-1.0, 1.0, 0.0, 1.0),
		vec4(-1.0, -1.0, 0.0,1.0)
	);
	gl_Position =g_world * vertices[gl_VertexID];
}