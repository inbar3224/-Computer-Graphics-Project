#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec3 vertexColor;

out vec3 geoPosition;
out vec3 geoColor;
out vec3 geoNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(vertexPosition, 1.0);
    geoColor = vertexColor;
    geoPosition = (model * vec4(vertexPosition, 1.0)).xyz;
    geoNormal = mat3(model) * vertexNormal;
}