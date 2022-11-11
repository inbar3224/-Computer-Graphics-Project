#version 330                                                                        

in vec4 FragPos;                                                                                                                                                                        
out vec4 fragColor;                                                                 
                                                                                 
void main(void)
{
	fragColor = vec4(vec3(FragPos.z),1);
}