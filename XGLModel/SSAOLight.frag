#version 460 core

in vec3 epos;
in vec3 enormal;
in vec2 ftexcoord;

uniform vec2 g_screensize;
uniform sampler2D g_colormap;
uniform sampler2D g_aomap;

out vec4 fragcolor;

void main(){
	
	vec2 aotexcoord = gl_FragCoord.xy / g_screensize;

	float ambient = texture(g_aomap, aotexcoord).r;

	fragcolor = ambient  * texture(g_colormap, ftexcoord);
//	fragcolor =vec4( ambient  );

}