#version 330                                                                        
                                                                                    
layout (location = 0) in vec3 position;                                             
                                                                                    
uniform mat4 gWVP;                                                                  
                                                                                    
out vec3 texCoord0;                                                                 
                                                                                    
void main()                                                                         
{                                                                                   
    vec4 wvpPos = gWVP * vec4(position, 1.0f);
    gl_Position = wvpPos.xyww;                                                     
    texCoord0 = position;                                                         
}
