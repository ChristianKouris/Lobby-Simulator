#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec2 textCoord;

uniform sampler2D tex;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{
   
    fragColor = texture(tex, textCoord);

}