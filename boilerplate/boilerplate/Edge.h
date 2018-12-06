#pragma once

#include"Geometry.h"

class Edge
{
	public:

	int k;
	int m;
	std::vector<glm::vec4> E;
	std::vector<float> U;
	std::vector<glm::vec4> C;

	Geometry curve;

	Edge(int k, int m, std::vector<glm::vec4> E, std::vector<float> U, std::vector<glm::vec4> C);
	void Select(bool selected);
	void Update(std::vector<glm::vec4> E);
	void Delete();
	void Add();
	Edge();
	~Edge();

};

