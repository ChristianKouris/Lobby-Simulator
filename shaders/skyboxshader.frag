#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader 
in vec3 text;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

uniform samplerCube sky;

void main()
{
      fragColor = texture(sky, text);
      
}