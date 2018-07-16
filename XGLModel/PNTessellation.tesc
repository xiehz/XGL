#version 410 core

layout(vertices = 1) out;

uniform float g_tesslevel;

struct OutputPatch{

	vec3 B300;
	vec3 B030;
	vec3 B003;
	vec3 B210;
	vec3 B120;
	vec3 B201;
	vec3 B021;
	vec3 B102;
	vec3 B012;
	vec3 B111;

	vec3 Normal[3];
	vec2 TexCoord[3];

};

in vec3 in_cs_world[];
in vec2 in_cs_texcoord[];
in vec3 in_cs_wnormal[];

out patch OutputPatch nPatch; 

vec3 projectToPlane(vec3 p, vec3 s, vec3 n )
{
	vec3 sp = p -s;
	float l = dot( sp, n);
	vec3 sn = n * l;
	return p - sn;//向量计算点
}

void bezier()
{
	nPatch.B030 = in_cs_world[0];
		nPatch.B003 = in_cs_world[1];
			nPatch.B300 = in_cs_world[2];

	    // Edges are names according to the opposing vertex
    vec3 EdgeB300 = nPatch.B003 - nPatch.B030;
    vec3 EdgeB030 = nPatch.B300 - nPatch.B003;
    vec3 EdgeB003 = nPatch.B030 - nPatch.B300;

    // Generate two midpoints on each edge
    nPatch.B021 = nPatch.B030 + EdgeB300 / 3.0;
    nPatch.B012 = nPatch.B030 + EdgeB300 * 2.0 / 3.0;
    nPatch.B102 = nPatch.B003 + EdgeB030 / 3.0;
    nPatch.B201 = nPatch.B003 + EdgeB030 * 2.0 / 3.0;
    nPatch.B210 = nPatch.B300 + EdgeB003 / 3.0;
    nPatch.B120 = nPatch.B300 + EdgeB003 * 2.0 / 3.0;

    // Project each midpoint on the plane defined by the nearest vertex and its normal
    nPatch.B021 = projectToPlane(nPatch.B021, nPatch.B030,
                                          nPatch.Normal[0]);
    nPatch.B012 = projectToPlane(nPatch.B012, nPatch.B003,
                                         nPatch.Normal[1]);
    nPatch.B102 = projectToPlane(nPatch.B102, nPatch.B003,
                                         nPatch.Normal[1]);
    nPatch.B201 = projectToPlane(nPatch.B201, nPatch.B300,
                                         nPatch.Normal[2]);
    nPatch.B210 = projectToPlane(nPatch.B210, nPatch.B300,
                                         nPatch.Normal[2]);
    nPatch.B120 = projectToPlane(nPatch.B120, nPatch.B030,
                                         nPatch.Normal[0]);

    // Handle the center
    vec3 Center = (nPatch.B003 + nPatch.B030 + nPatch.B300) / 3.0;
    nPatch.B111 = (nPatch.B021 + nPatch.B012 + nPatch.B102 +
                          nPatch.B201 + nPatch.B210 + nPatch.B120) / 6.0;
    nPatch.B111 += (nPatch.B111 - Center) / 2.0;
}

void main(){
	
	for(int i = 0 ; i < 3; ++i)
	{
		nPatch.Normal[i] = in_cs_wnormal[i];
		nPatch.TexCoord[i] = in_cs_texcoord[i];
	}

	bezier();

	

	gl_TessLevelOuter[0] = g_tesslevel;
		gl_TessLevelOuter[1] = g_tesslevel;
			gl_TessLevelOuter[2] = g_tesslevel;

				gl_TessLevelInner[0] = g_tesslevel;
}





