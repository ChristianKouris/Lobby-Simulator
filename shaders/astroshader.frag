#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec3 normOut;
in vec3 posOut;

uniform vec3 lDir;
uniform vec3 colorObj; 
uniform vec3 eyePos;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{

    vec3 lightDir = normalize( lDir ); 
    vec3 norm = normalize(normOut);

    float intensity = max(dot(norm, lightDir), 0.0);
    
    vec3 eyeDir = normalize(eyePos - posOut);
    vec3 eyeRef = reflect(-lDir, norm);

    float specular = 0.5 * pow(max(dot(eyeDir, eyeRef), 0.0), 32);
    //intensity = specular;
 
    //dicretize shading
    vec3 color = colorObj;
    if( intensity > 0.95 )
        color = vec3(1) * color;
    else if( intensity > 0.5 )
        color = vec3(0.7) * color;
    else if( intensity > 0.05 )
        color = vec3(0.35) * color;
    else
        color = vec3(0.1) * color;

    //Silhouette edge detection
    float edge = max(dot(norm, eyeDir), 0.0);
    float C = 0.5;
    if (edge < C)
        color = vec3(0);

    fragColor = vec4(color, 1.0);
    
}