#include "Edge.h"

#include "Geometry.h"

#include <vector>

Edge::Edge(int k, int m, std::vector<glm::vec4> E, std::vector<float> U, std::vector<glm::vec4> C)
{
	this->k = k;
	this->m = m;
	this->E = E;
	this->U = U;
	this->C = C;

	this->curve = Geometry::makeNURB(k, m, E, U, C, false);
}

void Edge::Select(bool selected) {
	this->curve = Geometry::makeNURB(k, m, E, U, C, selected);
}

void Edge::Update(std::vector<glm::vec4> E) {
	this->E = E;
	this->curve = Geometry::makeNURB(k, m, E, U, C, true);
}

void Edge::Delete() {
	this->k = k - 1;
	this->m = m - 1;
	U.clear();
	C.clear();

	for (int i = 0; i < (m + k + 1); i++) {
		int j = ((m + k + 1) / 2) - 1;
		if (i > j) {
			U.push_back(1);
		}
		else {
			U.push_back(0);
		}
	}

	C.resize(m + 1);

	this->curve = Geometry::makeNURB(k, m, E, U, C, true);
}

void Edge::Add() {
	this->k = k + 1;
	this->m = m + 1;
	U.clear();
	C.clear();

	for (int i = 0; i < (m + k + 1); i++) {
		int j = ((m + k + 1) / 2) - 1;
		if (i > j) {
			U.push_back(1);
		}
		else {
			U.push_back(0);
		}
	}

	C.resize(m + 1);

	this->curve = Geometry::makeNURB(k, m, E, U, C, true);
}

Edge::Edge()
{
}

Edge::~Edge()
{
}
