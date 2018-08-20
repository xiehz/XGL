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
  //------ Face normal
  //
  vec3 P0 = gl_in[0].gl_Position.xyz;
  vec3 P1 = gl_in[1].gl_Position.xyz;
  vec3 P2 = gl_in[2].gl_Position.xyz;
  
  vec3 V0 = P0 - P1;
  vec3 V1 = P2 - P1;
  
  // If the diff between V0 and V1 is too small, 
  // the normal will be incorrect as well as the deformation.
  //
  vec3 diff = V1 - V0;
  float diff_len = length(diff);
  
  vec3 N = normalize(cross(V1, V0));

  //------ Generate a new face along the direction of the face normal
  // only if diff_len is not too small.
  //
  if (length(diff_len) > 0.001)
  {
    int i;
    for(i=0; i<gl_in.length(); i++)
    {
      vec4 P = gl_in[i].gl_Position;
      vec3 N = normalize(cross(V1, V0));
      float len = sqrt(P.x*P.x + P.z*P.z);
//      float scale = 2.0 + 1.0 * cos(g_exploder*2.0 + len);

      P = vec4(P.xyz + (N  * g_exploder) + 
          (N * vec3(0.05, 0.05, 0.05)), 1.0);

      gl_Position = g_vp * P;

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

