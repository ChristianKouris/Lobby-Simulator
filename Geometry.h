#ifndef _Geometry_H_
#define _Geometry_H_

#include "Node.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class Geometry : public Node
{
private:
	glm::mat4 M;
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> norms;
	std::vector<glm::uvec3> indices;
	GLuint VAO, VBO, VBO2, EBO;
	GLuint shader;
	glm::vec3 color;

public:
	Geometry(std::string objFilename, GLuint shaderProgram, glm::vec3 objColor);
	~Geometry();

	void draw(glm::mat4& C);
	void update();

};

#endif