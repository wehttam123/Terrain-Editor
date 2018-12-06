#pragma once
#include "NURB.h"

#include <glm/glm.hpp>
#include <GL/glew.h>

#include <vector>

class Geometry {

public:
	Geometry();

	GLuint drawMode;

	GLuint vao;
	GLuint vertexBuffer;
	GLuint colourBuffer;
	std::vector<glm::vec3> verts;
	std::vector<glm::vec3> colours;
	glm::mat4 modelMatrix;

	static Geometry makeCircle(float radius, float x, float y, float r, float g, float b, bool fill);
	static Geometry makeSphere(float radius, float x, float y, float z, float r, float g, float b, bool fill);
	static Geometry Point(float x, float y, float z);

	//static Geometry makeHypocycloid(float r, float R, double n, float angle);
	//static Geometry makeLine(float x, float y, float x1, float y1);
	static Geometry makeBspline(int k, int m, std::vector<glm::vec3> E, std::vector<float> U, std::vector<glm::vec3> C, bool selected);
	static glm::vec3 makeSurface(int u, int v, int ku, int mu, int kv, int mv, std::vector<std::vector<glm::vec4>> E, std::vector<float> U, std::vector<float> V);
	static Geometry makeNURB(int k, int m, std::vector<glm::vec4> E, std::vector<float> U, std::vector<glm::vec4> C, bool selected);
	static Geometry makeCoons(NURB top, NURB bottom, NURB left, NURB right, int S, float R);

	static glm::vec3 Nurb(NURB n, float u);
	//static int N(int i, int k, float u, std::vector<float> U);
	static int delta(int m, int k, float u, std::vector<float> U);
};
