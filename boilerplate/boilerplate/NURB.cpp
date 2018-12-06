#include "NURB.h"

#include <vector>

NURB::NURB(int k, int m, std::vector<glm::vec4> E, std::vector<float> U, std::vector<glm::vec4> C)
{
	this->k = k;
	this->m = m;
	this->E = E;
	this->U = U;
	this->C = C;

}



NURB::NURB()
{
}


NURB::~NURB()
{
}
