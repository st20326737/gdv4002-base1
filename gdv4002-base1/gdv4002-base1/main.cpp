#include "Engine.h"


// Window size
const unsigned int initWidth = 512;
const unsigned int initHeight = 512;

// Function prototypes

void keyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods);
void updateScene();

int main(void) {

	int initResult = engineInit("GDV4002 - Applied Maths for Games");

	if (initResult != 0)
		return initResult; // exit if setup failed

	/*glm::vec3 v1 = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 v2 = glm::vec3(0.0f, 1.0f, 0.0f);

	auto dp = glm::dot(v1, v2);
	std::cout << "dp = " << dp << std::endl;*/

	//
	// 2. Main loop
	// 

	enterEngineMainLoop();

	glfwTerminate();
	return 0;
}


// Function to call to handle keyboard input
void keyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS) {

		// check which key was pressed...
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, true);
			break;

		default:
		{
		}
		}
	}
	else if (action == GLFW_RELEASE) {
		// handle key release events
	}
}


