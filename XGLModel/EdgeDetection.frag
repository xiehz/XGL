#version 330                                                                        
                                                                                    
uniform sampler2D g_sampler2d;                                                        
                                                                                    
in vec2 TexCoord;                                                                   
out vec4 FragColor;                                                                 
                                                                                    
void main()                                                                         
{                                                                                   
    FragColor = texture2D(g_sampler2d, TexCoord);                                     
                                                                                    
    if (FragColor.r >= 0.9 && FragColor.g >= 0.9 && FragColor.b >= 0.9) {           
        discard;                                                                    
    }               
	
}





















/*#version 330

in vec2 texcoord;

uniform sampler2D g_sampler2d;

out vec4 color;
void main(){

	color = texture2D(g_sampler2d, texcoord);
	if(color.r == 0 && color.g == 0 && color.b == 0)
		discard;

}*/