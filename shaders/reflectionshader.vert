#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

// Uniform vars
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

// Outputs of the vertex shader
out vec3 pos;
out vec3 norm;

void main()
{
    // OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
    gl_Position = projection * view * vec4(position, 1.0);
    norm = mat3(transpose(inverse(model))) * normal;
    pos = vec3(model * vec4(pos, 1.0f));

}