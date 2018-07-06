#version 330

uniform uint g_drawcallindex;
uniform uint g_objectindex;

out uvec3 vindex;

void main(){

	vindex = uvec3(g_objectindex, g_drawcallindex, gl_PrimitiveID + 1);

}