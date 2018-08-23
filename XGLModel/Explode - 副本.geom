#version 330                                                                        
layout(triangles) in;                                                                  
layout(triangle_strip) out;                                                         
layout(max_vertices = 3) out;                                                       
uniform mat4 g_vp;                                                                                                                        
uniform float g_exploder;                                                       
in Vertex{
	vec4 normal;
}Explode[];
out vec3 color;

void main()                                                                         
{                              
  vec3 p0 = gl_in[0].gl_Position.xyz;
  vec3 p1 = gl_in[1].gl_Position.xyz;
  vec3 p2 = gl_in[2].gl_Position.xyz;
  vec3 v0 = p0 - p1;
  vec3 v1 = p2 - p1;
  vec3 diff = v1 - v0;
  float diff_len = length(diff);
  vec3 N = normalize(cross(v1, v0));

  if (length(diff_len) > 0.001)
  {
    int i;
    for(i=0; i<gl_in.length(); i++)
    {
      vec4 p = gl_in[i].gl_Position;
      vec3 N = normalize(cross(v1, v0));
      float len = sqrt(p.x*p.x + p.z*p.z);

      p = vec4(p.xyz + (N  * g_exploder) + 
          (N * vec3(0.05, 0.05, 0.05)), 1.0);

      gl_Position = g_vp * p;

	  vec3 t = N + vec3(0, 0, 1);
	  if(length(t) - 0.00001 < 0)
	  {
			color = vec3(0.2, 0.2, 0.2);
	  }
	  else{
			color = abs(N);
	  }
      EmitVertex();
    }
    EndPrimitive();
  }                                                          
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

