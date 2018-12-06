#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <vector>

#include "Geometry.h"
#include "InputHandler.h"
#include "RenderEngine.h"

using namespace std;

class Program {

public:
	Program();
	void start();

	static int width;
	static int height;

	static string view;
	static string direction;

	static float rotatex;
	static float rotatey;
	static float translatex;
	static float translatey;
	static float scale;
	static float weight;
	static float resolution;
	static float x;
	static float y;

	static bool selected;
	static bool sketch;
	static bool PointSelected;
	static bool AddPoint;
	static bool Deleted;
	static bool DeleteSelected;
	static bool P;
	static bool L;
	static int S;

	Geometry test;
	Geometry patch;

private:
	GLFWwindow* window;
	RenderEngine* renderEngine;

	std::vector<Geometry*> objects;

	Geometry Spline;

	static void error(int error, const char* description);
	void setupWindow();
	void mainLoop();
};