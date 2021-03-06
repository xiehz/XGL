#version 330                                                                        
                                                                                    
layout (location = 0) in vec3 position;
layout(location = 1) in vec2 texcoord;
layout(location = 2) in vec3 normal;
                                                                
uniform mat4 g_world;

out Vertex{
	vec4 normal;
}Explode;
																
void main()                                                                         
{                                                                                   
    gl_Position = vec4(position, 1.0);                                              
	Explode.normal = transpose(inverse(g_world))* vec4(normal, 0);
}           