#ifndef _PARTICLE_SYSTEM_H_
#define _PARTICLE_SYSTEM_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

class ParticleSystem
{
private:
	struct Particle
	{
		glm::vec3 position, direction;
		float velocity;
		void initialize(glm::vec3 center) {
			position = center;
			direction = glm::normalize(glm::vec3(rand() % 1001 - 500, rand() % 1001 - 500, rand() % 1001 - 500));
			velocity = (float)(rand() % 1000) / 2000.0;
		}

		void update() {
			position += velocity * direction;
		}
	};
	
	static const int MAX_PARTICLES = 200;

	Particle particles[MAX_PARTICLES];
	glm::vec3 pos_data[MAX_PARTICLES];

	glm::vec3 color;
	int pointSize;
	double start;
	glm::mat4 model;
	GLuint VAO, VBO;

public:
	
	ParticleSystem(glm::vec3 center, glm::mat4 model, double start, bool isSpawned);
	~ParticleSystem();
	void draw(glm::mat4 view, glm::mat4 projection, GLuint shader);
	void update();
	double getStart();

};

#endif