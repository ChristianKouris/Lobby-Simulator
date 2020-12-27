#include "Light.h"

Light::Light(glm::vec3 dir)
{
	direction = glm::normalize(dir);
}
void Light::draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader)
{
	// Actiavte the shader program 
	glUseProgram(shader);

	// Get the shader variable locations and send the uniform data to the shader 
	glUniform3fv(glGetUniformLocation(shader, "lDir"), 1, glm::value_ptr(direction));

	glUseProgram(0);
}

void Light::update()
{
	
}

