#define _USE_MATH_DEFINES
#include "Geometry.h"

#include <cmath>
#include <vector>

Geometry::Geometry() {
	drawMode = GL_TRIANGLES;
	vao = 0;
	vertexBuffer = 0;
	colourBuffer = 0;
	modelMatrix = glm::mat4(1.f);
}

Geometry Geometry::makeCircle(float radius, float x1, float y1, float r, float g, float b, bool fill) {

	float x = 0.0;
	float y = 0.0;

	Geometry circle;

	for (double u = 0.0; u <= 2.0 * M_PI; u += 0.01) {
		x = radius * cos(u);
		y = radius * sin(u);

		circle.verts.push_back(glm::vec3(x + x1, y + y1, 0));
		circle.colours.push_back(glm::vec3(r, g, b));

	}

	if (fill) {
		circle.drawMode = GL_TRIANGLE_FAN;
	}
	else {
		circle.drawMode = GL_LINE_LOOP;
	}
	return circle;
}

Geometry Geometry::makeHypocycloid(float r, float R, double n, float angle) {

	float x;
	float y;
	float xr;
	float yr;

	Geometry Hypcycloid;

	for (double u = 0.0; u <= 2.0 * M_PI * n; u += 0.05) {

		// Parametric Equations
		x = ((R - r) * cos(u)) + (r * cos(((R - r) / r) * u));
		y = ((R - r) * sin(u)) - (r * sin(((R - r) / r) * u));

		// Adjust rotation
		xr = (x * cos(angle)) - (y * sin(angle));
		yr = (y * cos(angle)) + (x * sin(angle));

		Hypcycloid.verts.push_back(glm::vec3(xr, yr, 0));
		Hypcycloid.colours.push_back(glm::vec3(1.f, 0.f, 0.f));
	}

	Hypcycloid.drawMode = GL_LINE_STRIP;
	return Hypcycloid;
}

Geometry Geometry::makeLine(float x, float y, float x1, float y1) {

	Geometry line;

	line.verts.push_back(glm::vec3(x, y, 0));
	line.colours.push_back(glm::vec3(0.f, 0.f, 0.f));
	line.verts.push_back(glm::vec3(x1, y1, 0));
	line.colours.push_back(glm::vec3(0.f, 0.f, 0.f));

	line.drawMode = GL_LINE_STRIP;

	return line;
}

Geometry Geometry::makeBspline(int k, int m, std::vector<glm::vec2> E, std::vector<float> U, std::vector<glm::vec2> C) {

	int d;			//delta
	float omega;	//omega

	float x = 0.0;
	float y = 0.0;

	Geometry Spline;

	//Efficient Algorithm
	for (float u = 0.0; u <= 1.0; u += 0.01) {
		d = delta(m, k, u, U);

		for (int i = 0; i <= (k - 1); i++) {

			C.at(i).x = E.at(d - i).x;
			C.at(i).y = E.at(d - i).y;
		}

		for (int r = k; r >= 2; r--) {
			int i = d;
			for (int s = 0; s <= (r - 2); s++) {
				omega = (u - U.at(i)) / (U.at(i + r - 1) - U.at(i));
				C.at(s).x = (omega * C.at(s).x) + ((1-omega) * C.at(s+1).x);
				C.at(s).y = (omega * C.at(s).y) + ((1-omega) * C.at(s+1).y);

				i = i - 1;
			}
		}

		Spline.verts.push_back(glm::vec3(C.at(0).x, C.at(0).y, 0));
		Spline.colours.push_back(glm::vec3(1.f, 0.f, 0.f));
	}

	Spline.drawMode = GL_LINE_STRIP;
	return Spline;
}

int Geometry::N(int i, int k, float u, std::vector<float> U)
{
	int n = 0;

	if (k > 1) {
		try {
			float Ui = U.at(i);
			float Uik = U.at(i + k);
			float Ui1 = U.at(i + 1);
			float Uik1 = U.at(i + k - 1);
			n = (int)((((u - Ui) / (Uik1 - Ui))*N(i, k - 1, u, U)) + (((Uik - u) / (Uik - Ui1))*N(i + 1, k - 1, u, U)));
		} catch (...) {
			try {
				if (u >= U.at(i) and u <= U.at(i + 1)) {
					n = 1;
				}
			} catch (...) {
				n = 0;
			}
		}
	}
	else {
		try {
			if (u >= U.at(i) and u <= U.at(i + 1)) {
				n = 1;
			}
		}
		catch (...) {
			n = 0;
		}
	}

	return n;
}

int Geometry::delta(int m, int k, float u, std::vector<float> U)
{
	for (int i = 0; i <= (m + k - 1); i++) {
				if (u >= U.at(i) and u < U.at(i + 1)) {
					return i;
				}
	}

	return -1;
}