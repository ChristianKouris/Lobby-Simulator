#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#include "Object.h"
#include "stb_image.h"

#include <vector>
#include <string>
#include <iostream>

class Skybox : public Object
{
private:
	GLuint VAO;
	GLuint VBO, EBO;
	unsigned int cubemapTexture;

public:
	Skybox();
	~Skybox();

	void draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader);
	void update();
	unsigned int loadCubemap(std::vector<std::string> faces);
};

#endif