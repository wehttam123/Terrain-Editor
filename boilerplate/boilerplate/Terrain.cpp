#include "Terrain.h"
#include "Patch.h"
#include "Program.h"

#include <glm/glm.hpp>

Terrain::Terrain(int dimension)
{
	this->dimension = 1;

	Patch patch;
	vector<vector<Patch>> newTerrain(dimension, vector<Patch>(dimension, patch));

	this->patches = newTerrain;

	patches[0][0] = patch;
}

void Terrain::Grow()
{
	if (dimension < max) {
		dimension++;

		patches.resize(dimension);
		for (int i = 0; i < dimension; i++) {
			patches[i].resize(dimension);
		}
	}
}

void Terrain::Shrink()
{
	if (dimension > 1) {
		dimension--;

		patches.resize(dimension);
		for (int i = 0; i < dimension; ++i) {
			patches[i].resize(dimension);
		}
	}
}