#version 450 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout(location = 0) in vec3 position;
layout(location = 0) in vec3 normal;

out vec3 fragnormal;
out vec3 modelcoords;

void main() {
    gl_Position = projection * view * model * vec4(position, 1.0);
    fragnormal = inverse(transpose(mat3(model))) * normal;
    modelcoords = (model * vec4(position, 1.0)).xyz;
}
