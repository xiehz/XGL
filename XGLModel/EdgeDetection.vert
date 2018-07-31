#version 330                                                                        
                                                                                    
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texcoord;
layout (location = 2) in vec3 normal;
                           
uniform mat4 g_mvp;
uniform mat4 g_world;

out vec3 worldPos;
void main()                                                                         
{                                          
	vec4 p = vec4(pos,1.0);

    gl_Position = g_mvp * p;
	
	worldPos = (g_world * p).xyz;
}           