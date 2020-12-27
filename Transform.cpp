#include "Transform.h"

Transform::Transform(glm::mat4 matrix) {
	M = matrix;
}

Transform::~Transform() {
	
}

void Transform::draw(glm::mat4& C) {
	
	M = M * C;
	for (Node* n : childs) {
		n->draw(M);
	}

}

void Transform::update() {

}

void Transform::addChild(Node* n) {
	childs.push_back(n);
}