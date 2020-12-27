#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader 
in vec3 pos;
in vec3 norm;


// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

uniform vec3 camPos;
uniform samplerCube sky;

void main()
{
      vec3 I = normalize(pos - camPos);
      vec3 R = reflect(I, normalize(norm));
      fragColor = vec4(texture(sky, R).rgb, 1.0);
}