#include "Patch.h"

#include <vector>

Patch::Patch()
{
	int k = 3;
	int m = 2;
	std::vector<glm::vec4> E;
	std::vector<glm::vec4> C(3);
	std::vector<float> U;

	for (int i = 0; i < (m + k + 1); i++) {
		int j = ((m + k + 1) / 2) - 1;
		if (i > j) {
			U.push_back(1);
		}
		else {
			U.push_back(0);
		}
	}

	E.clear();
	E.push_back(glm::vec4(-1.0, 1.0, 0.0, 1.0));
	E.push_back(glm::vec4(0.0, 1.0, 0.5, 5.0));
	E.push_back(glm::vec4(1.0, 1.0, 0.0, 1.0));
	this->topEdge = Edge(k, m, E, U, C);

	E.clear();
	E.push_back(glm::vec4(-1.0, -1.0, 0.0, 1.0));
	E.push_back(glm::vec4(0.0, -1.0, 1.0, 1.0));
	E.push_back(glm::vec4(1.0, -1.0, 0.0, 1.0));
	this->bottomEdge = Edge(k, m, E, U, C);

	E.clear();
	E.push_back(glm::vec4(-1.0, 1.0, 0.0, 1.0));
	E.push_back(glm::vec4(-1.0, 0.0, 0.2, 1.0));
	E.push_back(glm::vec4(-1.0, -1.0, 0.0, 1.0));
	this->leftEdge = Edge(k, m, E, U, C);

	E.clear();
	E.push_back(glm::vec4(1.0, 1.0, 0.0, 1.0));
	E.push_back(glm::vec4(1.0, 0.0, -0.5, 1.0));
	E.push_back(glm::vec4(1.0, -1.0, 0.0, 1.0));
	this->rightEdge = Edge(k, m, E, U, C);

	this->length = 2.0;
}

Patch::~Patch()
{
}


