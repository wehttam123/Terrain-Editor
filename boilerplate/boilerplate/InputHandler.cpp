#define _USE_MATH_DEFINES

#include "InputHandler.h"
#include "Program.h"

RenderEngine* InputHandler::renderEngine;
int InputHandler::mouseOldX;
int InputHandler::mouseOldY;
bool InputHandler::mouseLeft;
bool InputHandler::mouseRight;
bool mouseLeft = false;
bool mouseRight = false;

// Must be called before processing any GLFW events
void InputHandler::setUp(RenderEngine* renderEngine) {
	InputHandler::renderEngine = renderEngine;
}

// Callback for key presses
void InputHandler::key(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE) {
		glfwDestroyWindow(window);
		glfwTerminate();
		exit(0);
	}

	if (key == GLFW_KEY_ENTER && action == GLFW_RELEASE) {
		if (Program::view == "Curve") {
			Program::view = "Terrain";
		} else if (Program::view == "Terrain") {
			Program::view = "Curve";
		}
	}


	if (Program::view == "Terrain") {
		if (key == GLFW_KEY_UP) {
			Program::direction = "up";
		}

		if (key == GLFW_KEY_DOWN) {
			Program::direction = "down";
		}

		if (key == GLFW_KEY_LEFT) {
			Program::direction = "left";
		}

		if (key == GLFW_KEY_RIGHT) {
			Program::direction = "right";
		}

		if (key == GLFW_KEY_S && action == GLFW_RELEASE) {
			if (Program::S >= 2) {
				Program::S = 0;
			}
			else {
				Program::S++;
			}
		}

		if (key == GLFW_KEY_P && action == GLFW_RELEASE) {
			if (Program::P == true) {
				Program::P = false;
			}
			else if (Program::P == false) {
				Program::P = true;
			}
		}

		if (key == GLFW_KEY_L && action == GLFW_RELEASE) {
			if (Program::L == true) {
				Program::L = false;
			}
			else if (Program::L == false) {
				Program::L = true;
			}
		}

		if (key == GLFW_KEY_R && action == GLFW_RELEASE) {

			if (Program::resolution <= 0.01) {
				Program::resolution = 0.08333333;
			}
			else if (Program::resolution <= 0.08333333) {
				Program::resolution = 0.1666666;
			}
			else if (Program::resolution <= 0.1666666) {
				Program::resolution = 0.333333;
			}
			else if (Program::resolution <= 0.5) {
				Program::resolution = 0.01;
			}

		}
	}
}

// Callback for mouse button presses
void InputHandler::mouse(GLFWwindow* window, int button, int action, int mods) {
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	Program::x = ((xpos / Program::width) * 2.1) - 1.05;
	Program::y = ((ypos / Program::height) * 1.18) + 0.41;

	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		if (GLFW_PRESS == action) {
			mouseRight = true;
			if (Program::PointSelected == true) {
				Program::DeleteSelected = true;
			}
		}
		else if (GLFW_RELEASE == action) {
			mouseRight = false;
		}
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (GLFW_PRESS == action) {
			mouseLeft = true;

			if (Program::view == "Curve") {
				Program::x = ((xpos / Program::width) * 2.1) - 1.05;
				Program::y = ((ypos / Program::height) * 1.18) + 0.41;
			}
		}
		else if (GLFW_RELEASE == action) {
			mouseLeft = false;
			if (Program::PointSelected == false && Program::Deleted == false) {
				Program::AddPoint = true;
			}
			Program::PointSelected = false;
			Program::Deleted = false;
		}
	}

	if (button == GLFW_MOUSE_BUTTON_MIDDLE)
	{
			if (Program::view == "Curve") {
				if (Program::PointSelected == false) {
					Program::x = ((xpos / Program::width) * 2.1) - 1.05;
					Program::y = ((ypos / Program::height) * 1.18) + 0.41;

					Program::sketch = true;
				}
			}
		if (GLFW_RELEASE == action) {
			Program::sketch = false;
		}
	}
}

// Callback for mouse motion
void InputHandler::motion(GLFWwindow* window, double x, double y) {




	if (Program::view == "Terrain") {
		if (mouseLeft == true) {
			Program::rotatex = Program::rotatex + ((x - mouseOldX) / Program::width) * 360.0;
			Program::rotatey = Program::rotatey + ((y - mouseOldY) / Program::height) * 360.0;
		}

		if (mouseRight == true) {
			Program::translatex = Program::translatex + ((x - mouseOldX) / Program::width) * 10.0;
			Program::translatey = Program::translatey - ((y - mouseOldY) / Program::height) * 10.0;
		}
	} 

	if (Program::view == "Curve") {
		Program::x = ((x / Program::width) * 2.1) - 1.05;
		Program::y = ((y / Program::height) * 1.18) + 0.41;
		if (mouseLeft == true) {

			Program::x = ((x / Program::width) * 2.1) - 1.05;
			Program::y = ((y / Program::height) * 1.18) + 0.41;
			//mouseLeft = false;
		}
		if (mouseRight == true) {
			//mouseRight = false;
		}
	}

	mouseOldX = x;
	mouseOldY = y;
}

// Callback for mouse scroll
void InputHandler::scroll(GLFWwindow* window, double x, double y) {
	if (Program::scale - y >= 1 && Program::scale + y <= 100) {
		Program::scale = Program::scale - y;
	}

	if (Program::PointSelected == true) {
		Program::weight = y / 10;
	}
}

// Callback for window reshape/resize
void InputHandler::reshape(GLFWwindow* window, int width, int height) {
	//renderEngine->setWindowSize(width, height);
	Program::width = width;
	Program::height = height;
}