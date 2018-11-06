#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <vector>

#include "Geometry.h"
#include "InputHandler.h"
#include "RenderEngine.h"

class Program {

public:
	Program();
	void start();

	static int width;
	static int height;

	static bool hide;
	static bool kPressed;
	static bool uPressed;
	static double u;	// Parameter
	static int k;		// Order
	static int m;		// # of Control Points - 1

	static std::vector<glm::vec2> E;	//Control Values
	static std::vector<float> U;			//Knot Values

private:
	GLFWwindow* window;
	RenderEngine* renderEngine;

	std::vector<Geometry*> objects;

	Geometry Spline;
	Geometry controlPoint;

	static void error(int error, const char* description);
	void setupWindow();
	void mainLoop();
};