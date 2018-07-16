#version 330                                                                        
                                                                                    
layout(points) in;                                                                  
layout(triangle_strip) out;                                                         
layout(max_vertices = 4) out;                                                       
                                                                                    
uniform mat4 g_vp;                                                                   
uniform vec3 g_eye;                                                            
uniform float g_billboardsize;                                                       
                                                                                    
out vec2 TexCoord;                                                                  
                                                                                    
void main()                                                                         
{                                                                                   
    vec3 Pos = gl_in[0].gl_Position.xyz;                                            
    vec3 toCamera = normalize(g_eye - Pos);                                    
    vec3 up = vec3(0.0, 1.0, 0.0);                                                  
    vec3 right = cross(toCamera, up) * g_billboardsize;                              
                                                                                    
    Pos -= right;                                                                   
    gl_Position = g_vp * vec4(Pos, 1.0);                                             
    TexCoord = vec2(0.0, 0.0);                                                      
    EmitVertex();                                                                   
                                                                                    
    Pos.y += g_billboardsize;                                                        
    gl_Position = g_vp * vec4(Pos, 1.0);                                             
    TexCoord = vec2(0.0, 1.0);                                                      
    EmitVertex();                                                                   
                                                                                    
    Pos.y -= g_billboardsize;                                                        
    Pos += right;                                                                   
    gl_Position = g_vp * vec4(Pos, 1.0);                                             
    TexCoord = vec2(1.0, 0.0);                                                      
    EmitVertex();                                                                   
                                                                                    
    Pos.y += g_billboardsize;                                                        
    gl_Position = g_vp * vec4(Pos, 1.0);                                             
    TexCoord = vec2(1.0, 1.0);                                                      
    EmitVertex();                                                                   
                                                                                    
    EndPrimitive();                                                                 
}                                                                                   









//---------------------------------------------------------------------------------
/*#version 330

layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 8) out;

uniform mat4 g_vp;
uniform vec3 g_eye;

out vec2 texcoord;

void main(){

	vec3 pos = gl_in[0].gl_Position.xyz;
	vec3 forward = normalize(g_eye - pos);
	vec3 up = vec3(0.0, 1.0, 0.0);
	vec3 right = cross(up, forward);
//	up = cross(forward, right);


	pos -= (right * 0.5);
    gl_Position = g_vp * vec4(pos, 1.0);
    texcoord = vec2(0.0, 0.0);
    EmitVertex();

    pos.y += 1.0;
    gl_Position = g_vp * vec4(pos, 1.0);
    texcoord = vec2(0.0, 1.0);
    EmitVertex();

    pos.y -= 1.0;
    pos += right;
    gl_Position = g_vp * vec4(pos, 1.0);
    texcoord = vec2(1.0, 0.0);
    EmitVertex();

    pos.y += 1.0;
    gl_Position = g_vp * vec4(pos, 1.0);
    texcoord = vec2(1.0, 1.0);
    EmitVertex();

    EndPrimitive();


	vec3 offset = vec3( 1.0, 0.0, 0.0);
	pos += offset;

	pos -= (right * 0.5);
    gl_Position = g_vp * vec4(pos, 1.0);
    texcoord = vec2(0.0, 0.0);
    EmitVertex();
	
	pos.y += 1.0;
    gl_Position = g_vp * vec4(pos, 1.0);
    texcoord = vec2(0.0, 1.0);
    EmitVertex();

    pos.y -= 1.0;
    pos += right;
    gl_Position = g_vp * vec4(pos, 1.0);
    texcoord = vec2(1.0, 0.0);
    EmitVertex();

    pos.y += 1.0;
    gl_Position = g_vp * vec4(pos, 1.0);
    texcoord = vec2(1.0, 1.0);
    EmitVertex();

	EndPrimitive();


}*/

