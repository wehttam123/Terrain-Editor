#pragma once

#include "Patch.h"
#include "Geometry.h"
#include "RenderEngine.h"

#include <glm/glm.hpp>

#include <vector>

using namespace std;

class Terrain
{
private:
	static const int max = 10;

public:

	int dimension;

	vector<vector<Patch>> patches;

	Terrain(int dimension);
	void Grow();
	void Shrink();

};

