#version 330 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 WorldPos0;

void main() {
    gl_Position = projection * view * model * vec4(Position, 1.0);
    WorldPos0 = gl_Position.xyz;
}