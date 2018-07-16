#version 410 core

layout(triangles, equal_spacing, ccw ) in ;

uniform mat4 g_view;
uniform mat4 g_pers;
uniform sampler2D g_heightmap;

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

in patch OutputPatch nPatch;



out vec3 in_fs_world;
out vec2 in_fs_texcoord;
out vec3 in_fs_tnormal;	
out float height;

vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2)
{
    return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2;
}

vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2)
{
    return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
} 

void main(){

    // Interpolate the attributes of the output vertex using the barycentric coordinates        
    in_fs_texcoord = interpolate2D(nPatch.TexCoord[0], nPatch.TexCoord[1], nPatch.TexCoord[2]);
    in_fs_tnormal = interpolate3D(nPatch.Normal[0], nPatch.Normal[1], nPatch.Normal[2]);
	 
                                                                                                
    // Displace the vertex along the normal        
    height = texture(g_heightmap, in_fs_texcoord.xy).x;   
                        

	float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;
    float w = gl_TessCoord.z;

    float uPow3 = pow(u, 3);
    float vPow3 = pow(v, 3);
    float wPow3 = pow(w, 3);
    float uPow2 = pow(u, 2);
    float vPow2 = pow(v, 2);
    float wPow2 = pow(w, 2);


	vec3 control = nPatch.B300 * wPow3 +
                    nPatch.B030 * uPow3 +
                    nPatch.B003 * vPow3 +
                    nPatch.B210 * 3.0 * wPow2 * u +
                    nPatch.B120 * 3.0 * w * uPow2 +
                    nPatch.B201 * 3.0 * wPow2 * v +
                    nPatch.B021 * 3.0 * uPow2 * v +
                    nPatch.B102 * 3.0 * w * vPow2 +
                    nPatch.B012 * 3.0 * u * vPow2 +
                    nPatch.B111 * 6.0 * w * u * v;

    gl_Position = g_pers * g_view * vec4(control, 1.0);

}