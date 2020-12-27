#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "Object.h"

#include <vector>

class Light : public Object
{
private:

	glm::vec3 direction;

public:
	Light(glm::vec3 dir);
	void draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader);
	void update();
};

#endif