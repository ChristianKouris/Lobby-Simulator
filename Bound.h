#ifndef _BOUND_H_
#define _BOUND_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

class Bound
{
private:
	std::vector<float> distances;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> centers;
	std::vector<float> radii;
	int pSize;
	int sSize;

public:
	Bound();
	void addPlane(float distance, glm::vec3 normal);
	void addSphere(glm::vec3 center, float radius);
	glm::vec3 environmentCollide(glm::vec3 center, float radius);
	glm::vec3 spheresCollide(glm::vec3 c1, float r1, glm::vec3 c2, float r2);

	
};

#endif