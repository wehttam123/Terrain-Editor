#define _USE_MATH_DEFINES

#include "Program.h"
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int Program::width;
int Program::height;

bool Program::hide;
bool Program::uPressed;
bool Program::kPressed;

double Program::u;	// Parameter
int Program::k;		// Order
int Program::m;		// # of Control Points - 1

std::vector<glm::vec2> Program::E;	//Control Values
std::vector<float> Program::U;		//Knot Values

Program::Program() {
	window = nullptr;
	renderEngine = nullptr;
}

// Error callback for glfw errors
void Program::error(int error, const char* description) {
	std::cerr << description << std::endl;
}

// Called to start the program
void Program::start() {
	setupWindow();
	GLenum err = glewInit();
	if (glewInit() != GLEW_OK) {
		std::cerr << glewGetErrorString(err) << std::endl;
	}

	//Program::rPressed = false;
	//Program::nPressed = false;

	cout << "--Parametric B-Splines--\n\n";

	renderEngine = new RenderEngine(window);
	InputHandler::setUp(renderEngine);
	mainLoop();
}

// Creates GLFW window for the program and sets callbacks for input
void Program::setupWindow() {
	glfwSetErrorCallback(Program::error);
	if (glfwInit() == 0) {
		exit(EXIT_FAILURE);
	}

	Program::width = 1280;
	Program::height = 720;

	glfwWindowHint(GLFW_SAMPLES, 16);
	window = glfwCreateWindow(Program::width, Program::height, "CPSC 589 Assignment #2 Parametric Curves", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // V-sync on

	glfwSetKeyCallback(window, InputHandler::key);
	glfwSetMouseButtonCallback(window, InputHandler::mouse);
	glfwSetCursorPosCallback(window, InputHandler::motion);
	glfwSetScrollCallback(window, InputHandler::scroll);
	glfwSetWindowSizeCallback(window, InputHandler::reshape);
}

// Main loop
void Program::mainLoop() {

	Program::hide = false;

	Program::u = 0.5;	// Parameter
	Program::k = 4;		// Order
	Program::m = 3;		// # of Control Points - 1

	std::vector<Geometry> P;			//Control Points
	std::vector<Geometry> G;			//Geometry Objects
	std::vector<glm::vec2> GP;			//Geometry Points

	E.clear();
	U.clear();

	E.push_back(glm::vec2(-4, 0));
	E.push_back(glm::vec2(-4, 4));
	E.push_back(glm::vec2(4, 4));
	E.push_back(glm::vec2(4, 0));

	std::vector<glm::vec2> C(E.size());	//Vertices

	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();
		glLineWidth(3);

		U.clear();
		for (int i = 0; i <= (m + k); i++) {
			if (i < k) {
				U.push_back(0.0);
			}
			//else if ((m + k) % 2 != 0) {
				//U.push_back(0.5);
			//}
			else {
				U.push_back(1.0);
			}
		}

		// Build B-spline
		Spline = Geometry::makeBspline(Program::k, Program::m, E, U, C);

		/*** Calculations for Additional objects in scene ***/

		//Geometry

		float x = 0.0;
		float y = 0.0;

		G.clear();
		P.clear();
		GP.clear();

		for (int i = 0; i <= (k - 1); i++) {

			C.at(i).x = E.at(Geometry::delta(m, k, u, U) - i).x;
			C.at(i).y = E.at(Geometry::delta(m, k, u, U) - i).y;
		}

		float omega;

		for (int r = k; r >= 2; r--) {
			int i = Geometry::delta(m, k, u, U);
			for (int s = 0; s <= (r - 2); s++) {
				omega = (u - U.at(i)) / (U.at(i + r - 1) - U.at(i));
				x = (omega * C.at(s).x) + ((1 - omega) * C.at(s + 1).x);
				y = (omega * C.at(s).y) + ((1 - omega) * C.at(s + 1).y);

				P.push_back(Geometry::makeCircle(0.15, x, y, 0.5, 0.5, 0.5, true));

				GP.push_back(glm::vec2(x, y));

				C.at(s).x = x;
				C.at(s).y = y;
				i = i - 1;
			}

			for (int i = 0; i < GP.size(); i++) {
				if (i > 0) {
					G.push_back(Geometry::makeLine(GP.at(i).x, GP.at(i).y, GP.at(i - 1).x, GP.at(i - 1).y));
				}
			}
			GP.clear();
		}

		for (int i = 0; i < E.size(); i++) {
			P.push_back(Geometry::makeCircle(0.2, E.at(i).x, E.at(i).y, 0.0, 0.0, 0.0, true));
			GP.push_back(glm::vec2(E.at(i).x, E.at(i).y));
		}

		for (int i = 0; i < GP.size(); i++) {
			if (i > 0) {
				G.push_back(Geometry::makeLine(GP.at(i).x, GP.at(i).y, GP.at(i - 1).x, GP.at(i - 1).y));
			}
		}

		if (Program::hide == false) {
			for (int i = 0; i < P.size(); i++) {
				renderEngine->assignBuffers(P.at(i));
				renderEngine->updateBuffers(P.at(i));
				objects.push_back(&P.at(i));
			}

			for (int i = 0; i < G.size(); i++) {
				renderEngine->assignBuffers(G.at(i));
				renderEngine->updateBuffers(G.at(i));
				objects.push_back(&G.at(i));
			}
		} else {
			P.clear();
			for (int i = 0; i < E.size(); i++) {
				P.push_back(Geometry::makeCircle(0.2, E.at(i).x, E.at(i).y, 0.0, 0.0, 0.0, true));
				GP.push_back(glm::vec2(E.at(i).x, E.at(i).y));
			}

			for (int i = 0; i < P.size(); i++) {
				renderEngine->assignBuffers(P.at(i));
				renderEngine->updateBuffers(P.at(i));
				objects.push_back(&P.at(i));
			}
		}

		//Render Spline
		renderEngine->assignBuffers(Spline);
		renderEngine->updateBuffers(Spline);
		objects.push_back(&Spline);

		renderEngine->render(objects, glm::mat4(1.f));
		glfwSwapBuffers(window);
	}

	// Clean up, program needs to exit
	glfwDestroyWindow(window);
	delete renderEngine;
	glfwTerminate();
}
