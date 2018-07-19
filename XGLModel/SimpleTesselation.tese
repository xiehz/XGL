#version 410 core

layout( isolines, equal_spacing, ccw) in;

uniform mat4 g_view;
uniform mat4 g_pers;

void main(){

	/*//������ϸ��
	gl_Position = g_pers * g_view * (gl_TessCoord.x * gl_in[0].gl_Position  + 
														 gl_TessCoord.y * gl_in[1].gl_Position +
														 gl_TessCoord.z * gl_in[2].gl_Position);*/
							
							//quadsϸ��
    float u = gl_TessCoord.x;
    float omu = 1 - u;    // omuΪ1��ȥ"u"
    float v = gl_TessCoord.y;
    float omv = 1 - v;    // omvΪ1��ȥ"v"

    gl_Position = g_pers * g_view * (omu * omv * gl_in[0].gl_Position +
                  u * omv * gl_in[1].gl_Position +
                  u * v * gl_in[2].gl_Position +
                  omu * v * gl_in[3].gl_Position);
				  

}