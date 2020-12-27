#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "Node.h"
#include <list>

class Transform : public Node
{
private:
	glm::mat4 M;
	std::list<Node*> childs;
public:
	Transform(glm::mat4 matrix);
	~Transform();

	void draw(glm::mat4& C);
	void update();

	void addChild(Node* n);
};

#endif