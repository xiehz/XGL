#version 330                                                                        
const int count = 12;
const float pi = 3.1415926;
const float a = 2 * pi / count;
layout(points) in;                                                                  
layout(triangle_strip) out;                                                         
layout(max_vertices = 26) out;                                                       
                                                                                    
uniform mat4 g_vp;          

in Borehole{
	float depth;
	int instance;
}borehole[];
                 
out vec2 texcoord;

void main()                                                                         
{   
    vec3 pos = gl_in[0].gl_Position.xyz;
	vec3 p;
	float f;
	for(int i = 0; i<= count; ++i){
		p.x = cos(a *i);
		p.y = 0;
		p.z = sin(a* i);
		
		gl_Position = g_vp *vec4(p + pos,1.0);
		f = float(i) / float(count);
		texcoord.x = f;
		texcoord.y = 1.0;
		EmitVertex();

		p.y =  - borehole[0].depth;
		gl_Position = g_vp *vec4(p + pos,1.0);
		texcoord.x = f;
		texcoord.y =1.0 - p.y / 100.0;
		EmitVertex();
	}

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

