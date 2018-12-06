#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>

#include <vector>

class NURB
{
public:

	int k;
	int m;
	std::vector<glm::vec4> E;
	std::vector<float> U;
	std::vector<glm::vec4> C;

	NURB(int k, int m, std::vector<glm::vec4> E, std::vector<float> U, std::vector<glm::vec4> C);

	NURB();
	~NURB();
};

