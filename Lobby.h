#ifndef _LOBBY_H_
#define _LOBBY_H_

#include "Object.h"
#include "stb_image.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class Lobby : public Object
{
private:
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textures;
	std::vector<glm::vec3> temp_verts;
	std::vector<glm::vec3> temp_norms;
	std::vector<glm::vec2> temp_texts;
	std::vector<unsigned int> vert_inds;
	std::vector<unsigned int> norm_inds;
	std::vector<unsigned int> text_inds;
	glm::vec3 color;
	unsigned int textId;
	GLuint VAO, VBO, VBO2;

public:
	Lobby();
	~Lobby();
	
	void draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader);
	void update();
	unsigned int loadTexture(std::string filename);
};

#endif
