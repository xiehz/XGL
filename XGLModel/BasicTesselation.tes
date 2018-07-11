#version 410 core

layout(triangles, equal_spacing, ccw ) in ;

uniform mat4 g_view;
uniform mat4 g_pers;
uniform sampler2D g_heightmap;

in vec3 in_es_world[];
in vec2 in_es_texcoord[];
in vec3 in_es_wnormal[];

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
    in_fs_texcoord = interpolate2D(in_es_texcoord[0], in_es_texcoord[1], in_es_texcoord[2]);    
    in_fs_tnormal = interpolate3D(in_es_wnormal[0], in_es_wnormal[1], in_es_wnormal[2]);            
    in_fs_tnormal = normalize(in_fs_tnormal);                                                     
    in_fs_world = interpolate3D(in_es_world[0], in_es_world[1], in_es_world[2]);    
                                                                                                
    // Displace the vertex along the normal        
    height = texture(g_heightmap, in_fs_texcoord.xy).x;   
    in_fs_world += in_fs_tnormal * pow(height, 3);                                
    gl_Position = g_pers * g_view * vec4(in_fs_world, 1.0);              

}