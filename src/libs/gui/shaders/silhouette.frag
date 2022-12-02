#version 330 core

out vec4 FragColor;
uniform vec3 objectColor;
uniform float alpha;



#include "compute_shading.frag"
#include "compute_shadow_factor.frag"

void main()
{
	vec3 ambient = computeAmbientComponent();
	vec3 diffuse = computeDiffuseComponent();
	vec3 specular = computeSpecularComponent();
	float shadowFactor = computeShadowMapShading();
	vec3 lighting = ambient + shadowFactor * (diffuse + specular);
	vec3 result =  lighting * objectColor;
    FragColor = vec4(result, alpha);
} 
