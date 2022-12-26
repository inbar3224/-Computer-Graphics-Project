#version 330 core

in vec3 geo_Normal;  
in vec3 geo_FragPos;
in vec4 lightSpacePos;

uniform vec3 lightPos; 
uniform vec3 camPos; 
uniform vec3 lightColor;

out vec4 FragColor;
uniform vec3 objectColor;
uniform float alpha;
uniform sampler2D texture_diffuse1;
in vec2 geo_TexCoords;
uniform bool use_textures;

vec3 computeAmbientComponent() {
	float ambientStrength = 0.3;
    return ambientStrength * lightColor;
}

vec3 getLightDir() {
	return normalize((camPos + lightPos) - geo_FragPos);
}

vec3 computeDiffuseComponent() {
	float diffuseStrength = 0.8;
	vec3 norm = normalize(geo_Normal);
	vec3 lightDir = getLightDir();
    float diff = max(dot(norm, lightDir), 0.0);
	return diffuseStrength * diff * lightColor;	
}

vec3 computeSpecularComponent() {
    float specularStrength = 0.5;
    vec3 viewDir = normalize(camPos - geo_FragPos);
	vec3 lightDir = getLightDir();	
	vec3 norm = normalize(geo_Normal);	
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    return specularStrength * spec * lightColor;
}

vec3 computeBasicShading() {
    vec3 ambient = computeAmbientComponent();
    vec3 diffuse = computeDiffuseComponent();
	vec3 specular = computeSpecularComponent();

	return ambient + diffuse + specular;
}

void main() {
	if (use_textures == false) {
		FragColor = vec4(computeBasicShading() * objectColor, alpha);
		//FragColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
	else {
		FragColor = vec4(computeBasicShading(), alpha) * texture(texture_diffuse1, geo_TexCoords);
	}
} 
