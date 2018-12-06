#pragma once

#include "Edge.h"

#include <glm/glm.hpp>

#include <vector>

class Patch
{
public:
	float length;

	Edge topEdge;
	Edge bottomEdge;
	Edge leftEdge;
	Edge rightEdge;

	Patch();
	~Patch();
};