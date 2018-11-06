#define _USE_MATH_DEFINES

#include "InputHandler.h"
#include "Program.h"

RenderEngine* InputHandler::renderEngine;
int InputHandler::mouseOldX;
int InputHandler::mouseOldY;
bool InputHandler::mouseLeft;
bool InputHandler::mouseRight;
int InputHandler::num = 0;
bool mouseLeft = false;
bool mouseRight = false;
bool A = false;
bool D = false;

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

	if (Program::uPressed) {
		if (key == GLFW_KEY_RIGHT) {
			if (Program::u <= 0.99) { Program::u = Program::u + 0.01; }
		}
		if (key == GLFW_KEY_LEFT) {
			if (Program::u >= 0.01) { Program::u = Program::u - 0.01; }
		}
	}
	else if (Program::kPressed) {
		if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
			if (Program::k <= Program::m) { Program::k = Program::k + 1; }
		}
		if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
			if (Program::k >= 0.0) { Program::k = Program::k - 1; }
		}
	}

	if (key == GLFW_KEY_G && action == GLFW_PRESS) {
		if (Program::hide) {
			Program::hide = false;
		}
		else {
			Program::hide = true;
		}
	}
	if (key == GLFW_KEY_H && action == GLFW_PRESS) {
		if (Program::hide) {
			Program::hide = false;
		}
		else {
			Program::hide = true;
		}
	}

	if (key == GLFW_KEY_K) {
		if (GLFW_PRESS == action)
			Program::kPressed = true;
		else if (GLFW_RELEASE == action)
			Program::kPressed = false;
	}

	if (key == GLFW_KEY_U) {
		if (GLFW_PRESS == action)
			Program::uPressed = true;
		else if (GLFW_RELEASE == action)
			Program::uPressed = false;
	}

	if (key == GLFW_KEY_0 && action == GLFW_PRESS) {
		num = 0;
	}
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		num = 1;
	}
	if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		num = 2;
	}
	if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
		num = 3;
	}
	if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
		num = 4;
	}
	if (key == GLFW_KEY_5 && action == GLFW_PRESS) {
		num = 5;
	}
	if (key == GLFW_KEY_6 && action == GLFW_PRESS) {
		num = 6;
	}
	if (key == GLFW_KEY_7 && action == GLFW_PRESS) {
		num = 7;
	}
	if (key == GLFW_KEY_8 && action == GLFW_PRESS) {
		num = 8;
	}
	if (key == GLFW_KEY_9 && action == GLFW_PRESS) {
		num = 9;
	}

	if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		A = true;
	}

	if (key == GLFW_KEY_D && action == GLFW_PRESS) {
		D = true;
	}
}

// Callback for mouse button presses
void InputHandler::mouse(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_RIGHT && GLFW_RELEASE == action) {
			mouseRight = true;
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (GLFW_PRESS == action)
			mouseLeft = true;
		else if (GLFW_RELEASE == action)
			mouseLeft = false;
	}
}

// Callback for mouse motion
void InputHandler::motion(GLFWwindow* window, double x, double y) {

	float x1 = (x - (Program::width / 2))/(32);
	float y1 = (-y + (Program::height / 2))/(32);

	if (mouseLeft == true) {
			Program::E.at(num).x = x1;
			Program::E.at(num).y = y1;
	}

	if (A == true) {

		Program::m++;
		Program::E.push_back(glm::vec2(x1, y1));
		A = false;
	}


	if (D == true) {

		Program::m--;
		Program::k--;
		Program::E.pop_back();
		D = false;
	}

}

// Callback for mouse scroll
void InputHandler::scroll(GLFWwindow* window, double x, double y) {
}

// Callback for window reshape/resize
void InputHandler::reshape(GLFWwindow* window, int width, int height) {
	renderEngine->setWindowSize(width, height);
	Program::width = width;
	Program::height = height;
}