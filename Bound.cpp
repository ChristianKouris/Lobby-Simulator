#include "Bound.h"

Bound::Bound()
{
	distances = std::vector<float>();
	normals = std::vector<glm::vec3>();
	centers = std::vector<glm::vec3>();
	radii = std::vector<float>();
	pSize = 0;
	sSize = 0;
}

void Bound::addPlane(float distance, glm::vec3 normal) {
	distances.push_back(distance);
	normals.push_back(normal);
	pSize++;
}

void Bound::addSphere(glm::vec3 center, float radius) {
	centers.push_back(center);
	radii.push_back(radius);
	sSize++;
}

glm::vec3 Bound::environmentCollide(glm::vec3 center, float radius)
{
	for (int i = 0; i < pSize; i++) {
		if (glm::dot(center, normals[i]) + distances[i] < radius) {
			return normals[i];
		}
	}
	for (int i = 0; i < sSize; i++) {
		glm::vec3 collisionNorm = spheresCollide(center, radius, centers[i], radii[i]);
		if (glm::length(collisionNorm) > 0.01) {
			return collisionNorm;
		}
	}
	return glm::vec3(0);
}

glm::vec3 Bound::spheresCollide(glm::vec3 c1, float r1, glm::vec3 c2, float r2)
{
	if (glm::length(c1 - c2) < r1 + r2) {
		return glm::normalize(c1 - c2);
	}
	return glm::vec3(0);
}