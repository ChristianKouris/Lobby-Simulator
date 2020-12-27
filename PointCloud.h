#ifndef _POINT_CLOUD_H_
#define _POINT_CLOUD_H_

#include "Object.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class PointCloud : public Object
{
private:
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> norms;
	std::vector<glm::uvec3> indicies;

	glm::vec3 ambient, diffuse, specular;
	GLint mode;

	GLuint VAO, VBO, VBO2, EBO;
	GLfloat pointSize;

public:
	PointCloud(std::string objFilename, GLfloat pointSize, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	~PointCloud();
	
	void draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader);
	void update();

	void updatePointSize(GLfloat size);
	void spin(float angle, glm::vec3 axis);
	void zoom(float factor);
	void shaderMode(GLuint shader);
};

#endif
