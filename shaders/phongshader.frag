#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec3 normOut;
in vec3 posOut;

uniform vec3 lPos;
uniform vec3 lColor;
uniform vec3 colorObj; 
uniform vec3 eyePos;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{
    float ambStr = 0.1;
    vec3 ambient = ambStr * lColor;

    vec3 norm = normalize(normOut);
    vec3 lDir = normalize(lPos - posOut);

    vec3 diffuse = max(dot(norm, lDir), 0.0) * lColor;
    
    vec3 eyeDir = normalize(eyePos - posOut);
    vec3 eyeRef = reflect(-lDir, norm);

    vec3 specular = 0.5 * pow(max(dot(eyeDir, eyeRef), 0.0), 32) * lColor;  

    vec3 result = (ambient + diffuse + specular) * colorObj;
    fragColor = vec4(result, 1.0);
    
}