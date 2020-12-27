#ifndef _ASTRO_STILL_H_
#define _ASTRO_STILL_H_

#include "Object.h"
#include <GLFW/glfw3.h>

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class AstroStill : public Object
{
private:
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> temp_verts;
	std::vector<glm::vec3> temp_norms;
	std::vector<unsigned int> vert_inds;
	std::vector<unsigned int> norm_inds;
	glm::vec3 color;
	glm::vec3 oldDir;
	glm::vec3 center;
	double timeToStop;
	double timeToStart;
	double startConst;
	double stopConst;
	float radius;
	GLuint VAO, VBO, VBO2;

public:
	AstroStill(glm::vec3 astro_color);
	~AstroStill();
	
	void draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader);
	void update();
	void move(glm::vec3 direction);
	void reflect(glm::vec3 norm);
	glm::vec3 getColor();
	glm::vec3 getCenter();
	float getRadius();
};

#endif
