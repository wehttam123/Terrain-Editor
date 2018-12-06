#define _USE_MATH_DEFINES

#include "Program.h"
#include "Terrain.h"
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int Program::width;
int Program::height;

string Program::view;
string Program::direction;

bool Program::P;
bool Program::L;
int Program::S;

float Program::rotatex;
float Program::rotatey;
float Program::translatex;
float Program::translatey;
float Program::scale;
float Program::weight;
float Program::resolution;
float Program::x;
float Program::y;

bool Program::selected;
bool Program::PointSelected;
bool Program::AddPoint;
bool Program::DeleteSelected;
bool Program::Deleted;
bool Program::sketch;

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

	cout << "--Terrain Editor--\n\n";

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

	Program::width = 1920;
	Program::height = 1080;

	glfwWindowHint(GLFW_SAMPLES, 16);
	window = glfwCreateWindow(Program::width, Program::height, "Terrain Editor", NULL, NULL);
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
	
	Program::view = "Terrain";
	Program::resolution = 0.1666666;
	Program::scale = 1.0f;
	Program::P = true;
	Program::L = true;
	Program::S = 0;
	Program::PointSelected = false;
	Program::DeleteSelected = false;
	Program::Deleted = false;
	Program::AddPoint = false;
	Program::sketch = false;

	int SelectedPoint = -1;
	std::vector<Geometry> CP;
	Terrain terrain(1);

	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();
		glLineWidth(2);
		glPatchParameteri(GL_PATCH_VERTICES, 3);
		objects.clear();
		CP.clear();
		glm::mat4 transformation = glm::mat4(1.0f);

		if (view == "Terrain") {

			if (Program::S < 2) {
				NURB top = NURB(terrain.patches[0][0].topEdge.k, terrain.patches[0][0].topEdge.m, terrain.patches[0][0].topEdge.E, terrain.patches[0][0].topEdge.U, terrain.patches[0][0].topEdge.C);
				NURB bottom = NURB(terrain.patches[0][0].bottomEdge.k, terrain.patches[0][0].bottomEdge.m, terrain.patches[0][0].bottomEdge.E, terrain.patches[0][0].bottomEdge.U, terrain.patches[0][0].bottomEdge.C);
				NURB left = NURB(terrain.patches[0][0].leftEdge.k, terrain.patches[0][0].leftEdge.m, terrain.patches[0][0].leftEdge.E, terrain.patches[0][0].leftEdge.U, terrain.patches[0][0].leftEdge.C);
				NURB right = NURB(terrain.patches[0][0].rightEdge.k, terrain.patches[0][0].rightEdge.m, terrain.patches[0][0].rightEdge.E, terrain.patches[0][0].rightEdge.U, terrain.patches[0][0].rightEdge.C);

				patch = Geometry::makeCoons(top, bottom, left, right, S, Program::resolution);

				renderEngine->assignBuffers(patch);
				renderEngine->updateBuffers(patch);
				objects.push_back(&patch);
			}

		if (direction == "up") {
			terrain.patches[0][0].topEdge.Select(true);
			terrain.patches[0][0].bottomEdge.Select(false);
			terrain.patches[0][0].leftEdge.Select(false);
			terrain.patches[0][0].rightEdge.Select(false);
		}
		else if (direction == "down") {
			terrain.patches[0][0].topEdge.Select(false);
			terrain.patches[0][0].bottomEdge.Select(true);
			terrain.patches[0][0].leftEdge.Select(false);
			terrain.patches[0][0].rightEdge.Select(false);
		}
		else if (direction == "left") {
			terrain.patches[0][0].topEdge.Select(false);
			terrain.patches[0][0].bottomEdge.Select(false);
			terrain.patches[0][0].leftEdge.Select(true);
			terrain.patches[0][0].rightEdge.Select(false);
		}
		else if (direction == "right") {
			terrain.patches[0][0].topEdge.Select(false);
			terrain.patches[0][0].bottomEdge.Select(false);
			terrain.patches[0][0].leftEdge.Select(false);
			terrain.patches[0][0].rightEdge.Select(true);
		}

		//Render Curves

		if (Program:: L) {
			renderEngine->assignBuffers(terrain.patches[0][0].topEdge.curve);
			renderEngine->updateBuffers(terrain.patches[0][0].topEdge.curve);
			objects.push_back(&terrain.patches[0][0].topEdge.curve);

			renderEngine->assignBuffers(terrain.patches[0][0].bottomEdge.curve);
			renderEngine->updateBuffers(terrain.patches[0][0].bottomEdge.curve);
			objects.push_back(&terrain.patches[0][0].bottomEdge.curve);

			renderEngine->assignBuffers(terrain.patches[0][0].rightEdge.curve);
			renderEngine->updateBuffers(terrain.patches[0][0].rightEdge.curve);
			objects.push_back(&terrain.patches[0][0].rightEdge.curve);

			renderEngine->assignBuffers(terrain.patches[0][0].leftEdge.curve);
			renderEngine->updateBuffers(terrain.patches[0][0].leftEdge.curve);
			objects.push_back(&terrain.patches[0][0].leftEdge.curve);
		}

		if (Program::P) {
			for (int i = 0; i < terrain.patches[0][0].topEdge.E.size(); i++)
			{
				CP.push_back(Geometry::Point(terrain.patches[0][0].topEdge.E[i].x, terrain.patches[0][0].topEdge.E[i].y, terrain.patches[0][0].topEdge.E[i].z));
			}

			for (int i = 0; i < terrain.patches[0][0].bottomEdge.E.size(); i++)
			{
				CP.push_back(Geometry::Point(terrain.patches[0][0].bottomEdge.E[i].x, terrain.patches[0][0].bottomEdge.E[i].y, terrain.patches[0][0].bottomEdge.E[i].z));
			}

			for (int i = 0; i < terrain.patches[0][0].rightEdge.E.size(); i++)
			{
				CP.push_back(Geometry::Point(terrain.patches[0][0].rightEdge.E[i].x, terrain.patches[0][0].rightEdge.E[i].y, terrain.patches[0][0].rightEdge.E[i].z));
			}

			for (int i = 0; i < terrain.patches[0][0].leftEdge.E.size(); i++)
			{
				CP.push_back(Geometry::Point(terrain.patches[0][0].leftEdge.E[i].x, terrain.patches[0][0].leftEdge.E[i].y, terrain.patches[0][0].leftEdge.E[i].z));
			}
		}

		transformation = glm::translate(transformation, glm::vec3(translatex, translatey, 0.0f));
		transformation = glm::rotate(transformation, glm::radians(rotatex), glm::vec3(0.0f, 1.0f, 0.0f));
		transformation = glm::rotate(transformation, glm::radians(rotatey), glm::vec3(1.0f, 0.0f, 0.0f));
		transformation = glm::scale(transformation, glm::vec3(scale, scale, scale));

		}
		else if (view == "Curve") {
			if (direction == "up") {

				if (InputHandler::mouseLeft) {
					for (int i = 0; i < terrain.patches[0][0].topEdge.E.size() - 1; i++)
					{
						float d = sqrt(pow((-Program::x - terrain.patches[0][0].topEdge.E[i].x) , 2) + pow(((-Program::y + 1.0) - terrain.patches[0][0].topEdge.E[i].z) , 2));
						if (d < 0.01) {
							Program::PointSelected = true;
							SelectedPoint = i;
						}
					}

					//test = Geometry::Point(-Program::x, terrain.patches[0][0].topEdge.E[0].y, -Program::y + 1.0);
					//renderEngine->assignBuffers(test);
					//renderEngine->updateBuffers(test);
					//objects.push_back(&test);
				}

				if (Program::PointSelected == true) {

					terrain.patches[0][0].topEdge.E[SelectedPoint].x = -Program::x;
					terrain.patches[0][0].topEdge.E[SelectedPoint].z = (-Program::y + 1.0);

					terrain.patches[0][0].topEdge.E[SelectedPoint].w = terrain.patches[0][0].topEdge.E[SelectedPoint].w + Program::weight;
					Program::weight = 0.0;

					terrain.patches[0][0].topEdge.Update(terrain.patches[0][0].topEdge.E);

					if (Program::DeleteSelected == true) {

						terrain.patches[0][0].topEdge.E.erase(terrain.patches[0][0].topEdge.E.begin() + SelectedPoint);
						terrain.patches[0][0].topEdge.Delete();

						Program::DeleteSelected = false;
						Program::PointSelected = false;
						Program::Deleted = true;
					}

				}
				else if (Program::sketch == true){
					int add = 1;
					for (int i = 0; i < terrain.patches[0][0].topEdge.E.size() - 1; i++)
					{
						if (terrain.patches[0][0].topEdge.E[i].x > -Program::x) {
							int add = i;
							break;
						}
					}

					terrain.patches[0][0].topEdge.E.insert(terrain.patches[0][0].topEdge.E.begin() + add, glm::vec4(-Program::x, terrain.patches[0][0].topEdge.E[0].y, -Program::y + 1.0, 1.0));

					terrain.patches[0][0].topEdge.Update(terrain.patches[0][0].topEdge.E);

					terrain.patches[0][0].topEdge.Add();

				}

				if (Program::AddPoint == true) {
					int add = 1;
					for (int i = 0; i < terrain.patches[0][0].topEdge.E.size() - 1; i++)
					{
						if (terrain.patches[0][0].topEdge.E[i].x > -Program::x) {
							int add = i;
							break;
						}
					}
					
					terrain.patches[0][0].topEdge.E.insert(terrain.patches[0][0].topEdge.E.begin() + add, glm::vec4(-Program::x, terrain.patches[0][0].topEdge.E[0].y, -Program::y + 1.0, 1.0));

					terrain.patches[0][0].topEdge.Update(terrain.patches[0][0].topEdge.E);

					terrain.patches[0][0].topEdge.Add();
					Program::AddPoint = false;
				}

				for (int i = 0; i < terrain.patches[0][0].topEdge.E.size(); i++)
				{
					CP.push_back(Geometry::Point(terrain.patches[0][0].topEdge.E[i].x, terrain.patches[0][0].topEdge.E[i].y, terrain.patches[0][0].topEdge.E[i].z));
				}

				for (int i = 0; i < CP.size(); i++) {
					renderEngine->assignBuffers(CP.at(i));
					renderEngine->updateBuffers(CP.at(i));
					objects.push_back(&CP.at(i));
				}


				renderEngine->assignBuffers(terrain.patches[0][0].topEdge.curve);
				renderEngine->updateBuffers(terrain.patches[0][0].topEdge.curve);
				objects.push_back(&terrain.patches[0][0].topEdge.curve);

				transformation = glm::rotate(transformation, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				transformation = glm::rotate(transformation, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				transformation = glm::translate(transformation, glm::vec3(0.0f, -17.0f, 0.0f));

			}
			else if (direction == "down") {

				test = Geometry::Point(Program::x, terrain.patches[0][0].bottomEdge.E[0].y, -Program::y + 1.0);
				renderEngine->assignBuffers(test);
				renderEngine->updateBuffers(test);
				objects.push_back(&test);

				renderEngine->assignBuffers(terrain.patches[0][0].bottomEdge.curve);
				renderEngine->updateBuffers(terrain.patches[0][0].bottomEdge.curve);
				objects.push_back(&terrain.patches[0][0].bottomEdge.curve);

				for (int i = 0; i < terrain.patches[0][0].bottomEdge.E.size(); i++)
				{
					CP.push_back(Geometry::Point(terrain.patches[0][0].bottomEdge.E[i].x, terrain.patches[0][0].bottomEdge.E[i].y, terrain.patches[0][0].bottomEdge.E[i].z));
				}

				for (int i = 0; i < CP.size(); i++) {
					renderEngine->assignBuffers(CP.at(i));
					renderEngine->updateBuffers(CP.at(i));
					objects.push_back(&CP.at(i));
				}

				transformation = glm::rotate(transformation, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				transformation = glm::translate(transformation, glm::vec3(0.0f, 17.0f, 0.0f));
				
			}
			else if (direction == "right") {

				test = Geometry::Point( terrain.patches[0][0].rightEdge.E[0].x, Program::x, -Program::y + 1.0);
				renderEngine->assignBuffers(test);
				renderEngine->updateBuffers(test);
				objects.push_back(&test);

				renderEngine->assignBuffers(terrain.patches[0][0].rightEdge.curve);
				renderEngine->updateBuffers(terrain.patches[0][0].rightEdge.curve);
				objects.push_back(&terrain.patches[0][0].rightEdge.curve);

				for (int i = 0; i < terrain.patches[0][0].rightEdge.E.size(); i++)
				{
					CP.push_back(Geometry::Point(terrain.patches[0][0].rightEdge.E[i].x, terrain.patches[0][0].rightEdge.E[i].y, terrain.patches[0][0].rightEdge.E[i].z));
				}

				for (int i = 0; i < CP.size(); i++) {
					renderEngine->assignBuffers(CP.at(i));
					renderEngine->updateBuffers(CP.at(i));
					objects.push_back(&CP.at(i));
				}

				
				transformation = glm::rotate(transformation, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				transformation = glm::rotate(transformation, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				transformation = glm::translate(transformation, glm::vec3(-17.0f, 0.0f, 0.0f));
			}
			else if (direction == "left") {

				test = Geometry::Point(terrain.patches[0][0].leftEdge.E[0].x, -Program::x, -Program::y + 1.0);
				renderEngine->assignBuffers(test);
				renderEngine->updateBuffers(test);
				objects.push_back(&test);

				renderEngine->assignBuffers(terrain.patches[0][0].leftEdge.curve);
				renderEngine->updateBuffers(terrain.patches[0][0].leftEdge.curve);
				objects.push_back(&terrain.patches[0][0].leftEdge.curve);

				for (int i = 0; i < terrain.patches[0][0].leftEdge.E.size(); i++)
				{
					CP.push_back(Geometry::Point(terrain.patches[0][0].leftEdge.E[i].x, terrain.patches[0][0].leftEdge.E[i].y, terrain.patches[0][0].leftEdge.E[i].z));
				}


				transformation = glm::rotate(transformation, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				transformation = glm::rotate(transformation, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				transformation = glm::translate(transformation, glm::vec3(17.0f, 0.0f, 0.0f));
			}

			transformation = glm::scale(transformation, glm::vec3(17.0, 17.0, 17.0));
		}

		// Render Control Points
		for (int i = 0; i < CP.size(); i++) {

			renderEngine->assignBuffers(CP.at(i));
			renderEngine->updateBuffers(CP.at(i));
			objects.push_back(&CP.at(i));
		}

		renderEngine->render(objects, transformation);
		glfwSwapBuffers(window);
	}

	// Clean up, program needs to exit
	glfwDestroyWindow(window);
	delete renderEngine;
	glfwTerminate();
}
