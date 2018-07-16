#version 410 core

layout(vertices = 3) out;

uniform vec3 g_eye;

in vec3 in_cs_world[];
in vec2 in_cs_texcoord[];
in vec3 in_cs_wnormal[];

out vec3 in_es_world[];
out vec2 in_es_texcoord[];
out vec3 in_es_wnormal[];

float GetTessLevel(float d0, float d1)                                            
{                                                                                               
    float avgd = (d0 + d1) / 2.0;                                          
                                                                                                
    if (avgd <= 2.0) {                                                                   
        return 200.0;                                                                            
    }                                                                                           
	   else if (avgd <= 4.0) {                                                              
        return 150.0;                                                                             
    }
    else if (avgd <= 6.0) {                                                              
        return 100.0;                                                                             
    }
	else if (avgd <= 8.0) {                                                              
        return 50.0;                                                                             
    }
	 else if (avgd <= 11.0) {                                                              
        return 3.0;                                                                             
    }
    else {                                                                                      
        return 1.0;                                                                             
    }                                                                                           
}  

void main(){

	in_es_texcoord[gl_InvocationID] = in_cs_texcoord[gl_InvocationID];
	in_es_wnormal[gl_InvocationID] = in_cs_wnormal[gl_InvocationID];
	in_es_world[gl_InvocationID] = in_cs_world[gl_InvocationID];

	float dist0 = distance(g_eye, in_es_world[0]);
	float dist1 = distance(g_eye, in_es_world[1]);
	float dist2 = distance(g_eye, in_es_world[2]);

	gl_TessLevelOuter[0] = GetTessLevel(dist1, dist2);
	gl_TessLevelOuter[1] = GetTessLevel(dist0, dist2);
	gl_TessLevelOuter[2] = GetTessLevel(dist0, dist1);

	gl_TessLevelInner[0] = gl_TessLevelOuter[2];

}





