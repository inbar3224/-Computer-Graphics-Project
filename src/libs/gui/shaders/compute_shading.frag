
in vec3 geo_Normal;  
in vec3 geo_FragPos;
in vec4 lightSpacePos;

uniform vec3 lightPos; 
uniform vec3 camPos; 
uniform vec3 lightColor;

vec3 computeAmbientComponent(){
	float ambientStrength = 0.3;
    return ambientStrength * lightColor;
}

vec3 getLightDir(){
	return normalize((camPos + lightPos) - geo_FragPos);
}

vec3 computeDiffuseComponent(){
	float diffuseStrength = 0.8;
	vec3 norm = normalize(geo_Normal);
	vec3 lightDir = getLightDir();
    float diff = max(dot(norm, lightDir), 0.0);
	return diffuseStrength * diff * lightColor;	
}

vec3 computeSpecularComponent(){
    float specularStrength = 0.5;
    vec3 viewDir = normalize(camPos - geo_FragPos);
	vec3 lightDir = getLightDir();	
	vec3 norm = normalize(geo_Normal);	
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    return specularStrength * spec * lightColor;
}

vec3 computeBasicShading(){
    vec3 ambient = computeAmbientComponent();
    vec3 diffuse = computeDiffuseComponent();
	vec3 specular = computeSpecularComponent();

	return ambient + diffuse + specular;
}