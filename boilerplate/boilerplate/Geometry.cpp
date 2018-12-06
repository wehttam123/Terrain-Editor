#define _USE_MATH_DEFINES
#include "Geometry.h"
#include "NURB.h"

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

Geometry Geometry::Point(float x, float y, float z) {

	Geometry point;

	point.verts.push_back(glm::vec3(x, y, z));
	point.colours.push_back(glm::vec3(0.f, 0.f, 0.f));

	point.drawMode = GL_POINTS;

	return point;
}

Geometry Geometry::makeSphere(float radius, float x1, float y1, float z1, float r, float g, float b, bool fill) {

	float x = 0.0;
	float y = 0.0;
	float z = 0.0;

	Geometry sphere;

	for (double u = 0.0; u <= 2.0 * M_PI; u += 0.5) {
		for (double v = 0.0; v <= 2.0 * M_PI; v += 0.5) {
			x = radius * sin(u) * cos(v);
			y = radius * cos(u) * cos(v);
			z = radius * sin(v);

			sphere.verts.push_back(glm::vec3(x + x1, y + y1, z + z1));
			sphere.colours.push_back(glm::vec3(r, g, b));

		}
	}

	if (fill) {
		sphere.drawMode = GL_TRIANGLE_FAN;
	}
	else {
		sphere.drawMode = GL_LINE_LOOP;
	}
	return sphere;
}

Geometry Geometry::makeBspline(int k, int m, std::vector<glm::vec3> E, std::vector<float> U, std::vector<glm::vec3> C, bool selected) {

	int d;			//delta
	float omega;	//omega

	float x = 0.0;
	float y = 0.0;
	float z = 0.0;

	Geometry Spline;

	//Efficient Algorithm
	for (float u = 0.0; u <= 1.0; u += 0.01) {
		d = delta(m, k, u, U);

		for (int i = 0; i <= (k - 1); i++) {

			C.at(i).x = E.at(d - i).x;
			C.at(i).y = E.at(d - i).y;
			C.at(i).z = E.at(d - i).z;
		}

		for (int r = k; r >= 2; r--) {
			int i = d;
			for (int s = 0; s <= (r - 2); s++) {
				omega = (u - U.at(i)) / (U.at(i + r - 1) - U.at(i));
				C.at(s).x = (omega * C.at(s).x) + ((1 - omega) * C.at(s + 1).x);
				C.at(s).y = (omega * C.at(s).y) + ((1 - omega) * C.at(s + 1).y);
				C.at(s).z = (omega * C.at(s).z) + ((1 - omega) * C.at(s + 1).z);

				i = i - 1;
			}
		}

		Spline.verts.push_back(glm::vec3(C.at(0).x, C.at(0).y, C.at(0).z));

		if (selected) {
			Spline.colours.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
		}
		else {
			Spline.colours.push_back(glm::vec3(0.f, 0.5f, 0.f));
		}
	}

	Spline.drawMode = GL_LINE_STRIP;
	return Spline;
}

Geometry Geometry::makeCoons(NURB top, NURB bottom, NURB left, NURB right, int S, float R) {

	float x = 0.0;
	float y = 0.0;
	float z = 0.0;

	std::vector<std::vector<glm::vec3>> Mesh;
	std::vector<glm::vec3> Meshrow;
	glm::vec3 surface;
	glm::vec3 color = glm::vec3(0.f, 0.5f, 0.f);
	Geometry patch;

	for (float u = 0.0; u <= 1.0; u += R) {

		for (float v = 0.0; v <= 1.0; v += R) {

			//Coons calculation

			//Ruled Surface 1
			surface = ((1 - v) * Nurb(top, u)) + (v * Nurb(bottom, u));

			//Ruled Surface 2
			surface = surface + ((1 - u) * Nurb(left, v)) + (u * Nurb(right, v));

			//Points
			glm::vec3 D = glm::vec4(-1.0, 1.0, 0.0, 1.0);
			glm::vec3 A = glm::vec4(-1.0, -1.0, 0.0, 1.0);
			glm::vec3 C = glm::vec4(1.0, 1.0, 0.0, 1.0);
			glm::vec3 B = glm::vec4(1.0, -1.0, 0.0, 1.0);

			//Bilinear Patch
			surface = surface - ((((1 - u) * (1 - v)) * D) + ((u * (1 - v)) * C) + ((v * (1 - u)) * A) + ((u * v) * B));

			Meshrow.push_back(surface);
		}
		Mesh.push_back(Meshrow);
		Meshrow.clear();
	}

	//Organize Points for rendering
	for (int u = 0; u < Mesh.size()-1; u+=1) {
		for (int v = 0; v < Mesh[u].size()-1; v+=1) {

			patch.verts.push_back(Mesh[u+1][v+1]);
			patch.colours.push_back(color);
			patch.verts.push_back(Mesh[u][v+1]);
			patch.colours.push_back(color);
			patch.verts.push_back(Mesh[u][v]);
			patch.colours.push_back(color);
			patch.verts.push_back(Mesh[u + 1][v]);
			patch.colours.push_back(color);
		}
	}

	if (S == 0) {
		patch.drawMode = GL_POINTS;
	}
	else {
		patch.drawMode = GL_TRIANGLE_STRIP;
	}

	return patch;
}

Geometry Geometry::makeNURB(int k, int m, std::vector<glm::vec4> E, std::vector<float> U, std::vector<glm::vec4> C, bool selected) {

	int d;			//delta
	float omega;	//omega

	float x = 0.0;
	float y = 0.0;
	float z = 0.0;

	Geometry Spline;

	//Efficient Algorithm
	for (float u = 0.0; u <= 1.0; u += 0.01) {

		x = 0.0;
		y = 0.0;
		z = 0.0;

		d = delta(m, k, u, U);

		for (int i = 0; i <= (k - 1); i++) {

			C.at(i).x = E.at(d - i).x * E.at(d - i).w;
			C.at(i).y = E.at(d - i).y * E.at(d - i).w;
			C.at(i).z = E.at(d - i).z * E.at(d - i).w;
		}

		for (int r = k; r >= 2; r--) {
			int i = d;
			for (int s = 0; s <= (r - 2); s++) {
				omega = (u - U.at(i)) / (U.at(i + r - 1) - U.at(i));
				C.at(s).x = (omega * C.at(s).x) + ((1 - omega) * C.at(s + 1).x);
				C.at(s).y = (omega * C.at(s).y) + ((1 - omega) * C.at(s + 1).y);
				C.at(s).z = (omega * C.at(s).z) + ((1 - omega) * C.at(s + 1).z);

				i = i - 1;
			}
		}

		x = C.at(0).x;
		y = C.at(0).y;
		z = C.at(0).z;

		for (int i = 0; i <= (k - 1); i++) {

			C.at(i).x = E.at(d - i).w;
			C.at(i).y = E.at(d - i).w;
			C.at(i).z = E.at(d - i).w;
		}

		for (int r = k; r >= 2; r--) {
			int i = d;
			for (int s = 0; s <= (r - 2); s++) {
				omega = (u - U.at(i)) / (U.at(i + r - 1) - U.at(i));
				C.at(s).x = (omega * C.at(s).x) + ((1 - omega) * C.at(s + 1).x);
				C.at(s).y = (omega * C.at(s).y) + ((1 - omega) * C.at(s + 1).y);
				C.at(s).z = (omega * C.at(s).z) + ((1 - omega) * C.at(s + 1).z);

				i = i - 1;
			}
		}

		x = x / C.at(0).x;
		y = y / C.at(0).y;
		z = z / C.at(0).z;

		Spline.verts.push_back(glm::vec3(x, y, z));

		if (selected) {
			Spline.colours.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
		}
		else {
			Spline.colours.push_back(glm::vec3(0.f, 0.5f, 0.f));
		}
	}

	Spline.drawMode = GL_LINE_STRIP;
	return Spline;
}

glm::vec3 Geometry::makeSurface(int u, int v, int ku, int mu, int kv, int mv, std::vector<std::vector<glm::vec4>> E, std::vector<float> U, std::vector<float> V) {

	std::vector<glm::vec4> D(E.size());
	std::vector<glm::vec4> C(E.size());

	int du;			//delta U
	int dv;			//delta V
	float omega;	//omega

	float x = 0.0;
	float y = 0.0;
	float z = 0.0;

		du = delta(mu, ku, u, U);
		dv = delta(mv, kv, v, V);

		for (int i = 0; i <= (ku - 1); i++) {
			for (int j = 0; j <= (kv - 1); j++) {

				D.at(i).x = E.at(du - i).at(dv - j).y * E.at(du - i).at(dv - j).w;
				D.at(i).y = E.at(du - i).at(dv - j).y * E.at(du - i).at(dv - j).w;
				D.at(i).z = E.at(du - i).at(dv - j).z * E.at(du - i).at(dv - j).w;
			}

			for (int r = kv; r >= 2; r--) {
				int j = dv;
				for (int s = 0; s <= (r - 2); s++) {
					omega = (v - V.at(j)) / (V.at(j + r - 1) - V.at(j));
					D.at(s).x = (omega * D.at(s).x) + ((1 - omega) * D.at(s + 1).x);
					D.at(s).y = (omega * D.at(s).y) + ((1 - omega) * D.at(s + 1).y);
					D.at(s).z = (omega * D.at(s).z) + ((1 - omega) * D.at(s + 1).z);

					j = j - 1;
				}
			}

			C.at(i).x = D.at(0).x;
			C.at(i).y = D.at(0).y;
			C.at(i).z = D.at(0).z;
		}

		for (int r = ku; r >= 2; r--) {
			int i = du;
			for (int s = 0; s <= (r - 2); s++) {
				omega = (u - U.at(i)) / (U.at(i + r - 1) - U.at(i));
				C.at(s).x = (omega * C.at(s).x) + ((1 - omega) * C.at(s + 1).x);
				C.at(s).y = (omega * C.at(s).y) + ((1 - omega) * C.at(s + 1).y);
				C.at(s).z = (omega * C.at(s).z) + ((1 - omega) * C.at(s + 1).z);

				i = i - 1;
			}
		}

		x = C.at(0).x;
		y = C.at(0).y;
		z = C.at(0).z;

		for (int i = 0; i <= (ku - 1); i++) {
			for (int j = 0; j <= (kv - 1); j++) {

				D.at(i).x = E.at(du - i).at(dv - j).w;
				D.at(i).y = E.at(du - i).at(dv - j).w;
				D.at(i).z = E.at(du - i).at(dv - j).w;
			}

			for (int r = kv; r >= 2; r--) {
				int j = dv;
				for (int s = 0; s <= (r - 2); s++) {
					omega = (v - V.at(j)) / (V.at(j + r - 1) - V.at(j));
					D.at(s).x = (omega * D.at(s).x) + ((1 - omega) * D.at(s + 1).x);
					D.at(s).y = (omega * D.at(s).y) + ((1 - omega) * D.at(s + 1).y);
					D.at(s).z = (omega * D.at(s).z) + ((1 - omega) * D.at(s + 1).z);

					j = j - 1;
				}
			}

			C.at(i).x = D.at(0).x;
			C.at(i).y = D.at(0).y;
			C.at(i).z = D.at(0).z;
		}

		for (int r = ku; r >= 2; r--) {
			int i = du;
			for (int s = 0; s <= (r - 2); s++) {
				omega = (u - U.at(i)) / (U.at(i + r - 1) - U.at(i));
				C.at(s).x = (omega * C.at(s).x) + ((1 - omega) * C.at(s + 1).x);
				C.at(s).y = (omega * C.at(s).y) + ((1 - omega) * C.at(s + 1).y);
				C.at(s).z = (omega * C.at(s).z) + ((1 - omega) * C.at(s + 1).z);

				i = i - 1;
			}
		}

		x = x / C.at(0).x;
		y = y / C.at(0).y;
		z = z / C.at(0).z;

	return glm::vec3(x, y, z);
}


glm::vec3 Geometry::Nurb(NURB NURB, float u) {

	int k = NURB.k;
	int m = NURB.m;
	std::vector<glm::vec4> E = NURB.E;
	std::vector<float> U = NURB.U;
	std::vector<glm::vec4> C = NURB.C;

	int d;			//delta
	float omega;	//omega

	float x = 0.0;
	float y = 0.0;
	float z = 0.0;


		x = 0.0;
		y = 0.0;
		z = 0.0;

		d = delta(m, k, u, U);

		for (int i = 0; i <= (k - 1); i++) {

			C.at(i).x = E.at(d - i).x * E.at(d - i).w;
			C.at(i).y = E.at(d - i).y * E.at(d - i).w;
			C.at(i).z = E.at(d - i).z * E.at(d - i).w;
		}

		for (int r = k; r >= 2; r--) {
			int i = d;
			for (int s = 0; s <= (r - 2); s++) {
				omega = (u - U.at(i)) / (U.at(i + r - 1) - U.at(i));
				C.at(s).x = (omega * C.at(s).x) + ((1 - omega) * C.at(s + 1).x);
				C.at(s).y = (omega * C.at(s).y) + ((1 - omega) * C.at(s + 1).y);
				C.at(s).z = (omega * C.at(s).z) + ((1 - omega) * C.at(s + 1).z);

				i = i - 1;
			}
		}

		x = C.at(0).x;
		y = C.at(0).y;
		z = C.at(0).z;

		for (int i = 0; i <= (k - 1); i++) {

			C.at(i).x = E.at(d - i).w;
			C.at(i).y = E.at(d - i).w;
			C.at(i).z = E.at(d - i).w;
		}

		for (int r = k; r >= 2; r--) {
			int i = d;
			for (int s = 0; s <= (r - 2); s++) {
				omega = (u - U.at(i)) / (U.at(i + r - 1) - U.at(i));
				C.at(s).x = (omega * C.at(s).x) + ((1 - omega) * C.at(s + 1).x);
				C.at(s).y = (omega * C.at(s).y) + ((1 - omega) * C.at(s + 1).y);
				C.at(s).z = (omega * C.at(s).z) + ((1 - omega) * C.at(s + 1).z);

				i = i - 1;
			}
		}

		x = x / C.at(0).x;
		y = y / C.at(0).y;
		z = z / C.at(0).z;


	return glm::vec3(x, y, z);
}

int Geometry::delta(int m, int k, float u, std::vector<float> U)
{
	for (int i = 0; i <= (m + k - 1); i++) {
		if (u >= U.at(i) && u < U.at(i + 1)) {
			return i;
		}
	}

	return -1;
}

//int Geometry::N(int i, int k, float u, std::vector<float> U)
//{
//	int n = 0;
//
//	if (k > 1) {
//		try {
//			float Ui = U.at(i);
//			float Uik = U.at(i + k);
//			float Ui1 = U.at(i + 1);
//			float Uik1 = U.at(i + k - 1);
//			n = (int)((((u - Ui) / (Uik1 - Ui))*N(i, k - 1, u, U)) + (((Uik - u) / (Uik - Ui1))*N(i + 1, k - 1, u, U)));
//		} catch (...) {
//			try {
//				if (u >= U.at(i) && u <= U.at(i + 1)) {
//					n = 1;
//				}
//			} catch (...) {
//				n = 0;
//			}
//		}
//	}
//	else {
//		try {
//			if (u >= U.at(i) && u <= U.at(i + 1)) {
//				n = 1;
//			}
//		}
//		catch (...) {
//			n = 0;
//		}
//	}
//
//	return n;
//}

//Geometry Geometry::makeHypocycloid(float r, float R, double n, float angle) {
//
//	float x;
//	float y;
//	float xr;
//	float yr;
//
//	Geometry Hypcycloid;
//
//	for (double u = 0.0; u <= 2.0 * M_PI * n; u += 0.05) {
//
//		// Parametric Equations
//		x = ((R - r) * cos(u)) + (r * cos(((R - r) / r) * u));
//		y = ((R - r) * sin(u)) - (r * sin(((R - r) / r) * u));
//
//		// Adjust rotation
//		xr = (x * cos(angle)) - (y * sin(angle));
//		yr = (y * cos(angle)) + (x * sin(angle));
//
//		Hypcycloid.verts.push_back(glm::vec3(xr, yr, 0));
//		Hypcycloid.colours.push_back(glm::vec3(1.f, 0.f, 0.f));
//	}
//
//	Hypcycloid.drawMode = GL_LINE_STRIP;
//	return Hypcycloid;
//}
//
//Geometry Geometry::makeLine(float x, float y, float x1, float y1) {
//
//	Geometry line;
//
//	line.verts.push_back(glm::vec3(x, y, 0));
//	line.colours.push_back(glm::vec3(0.f, 0.f, 0.f));
//	line.verts.push_back(glm::vec3(x1, y1, 0));
//	line.colours.push_back(glm::vec3(0.f, 0.f, 0.f));
//
//	line.drawMode = GL_LINE_STRIP;
//
//	return line;
//}