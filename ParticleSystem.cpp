#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(glm::vec3 center, glm::mat4 model, double start, bool isSpawned)
	:start(start)
{
	if (isSpawned) {
		color = glm::vec3(0.9);
		pointSize = 1;
	}
	else {
		color = glm::vec3(0.9, 0.1, 0.1);
		pointSize = 10;
	}
	for (int i = 0; i < MAX_PARTICLES; i++) {
		Particle part; 
		part.initialize(center);
		particles[i] = part;
		pos_data[i] = part.position;
	}

	model = glm::mat4(1);

	// Generate a Vertex Array (VAO) and Vertex Buffer Object (VBO)
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind VAO
	glBindVertexArray(VAO);

	// Bind VBO to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * MAX_PARTICLES, &pos_data[0], GL_STATIC_DRAW);
	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

ParticleSystem::~ParticleSystem()
{
	// Delete the VBO and the VAO.
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void ParticleSystem::draw(glm::mat4 view, glm::mat4 projection, GLuint shader) {
	glUseProgram(shader);
	// Get the shader variable locations and send the uniform data to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(color));
	// Bind the VAO
	glBindVertexArray(VAO);

	// Set point size
	glPointSize(pointSize);

	// Draw the points 
	glDrawArrays(GL_POINTS, 0, MAX_PARTICLES);

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}

void ParticleSystem::update() {
	for (int i = 0; i < MAX_PARTICLES; i++) {
		particles[i].update();
	}
}

double ParticleSystem::getStart() {
	return start;
}