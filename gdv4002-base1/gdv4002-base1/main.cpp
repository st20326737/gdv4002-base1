#include "Engine.h"

// Function prototypes

//void myKeyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods);

//void updateScene();


int main(void) {

	int initResult = engineInit("GDV4002 - Applied Maths for Games");

	if (initResult != 0)
		return initResult; // exit if setup failed

	engineMainLoop();

	glfwTerminate();
	return 0;
}


// Function to call to handle keyboard input
//void myKeyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods)
//{
//	if (action == GLFW_PRESS) {
//
//		// check which key was pressed...
//		switch (key)
//		{
//		case GLFW_KEY_ESCAPE:
//			glfwSetWindowShouldClose(window, true);
//			break;
//
//		default:
//		{
//		}
//		}
//	}
//	else if (action == GLFW_RELEASE) {
//		// handle key release events
//	}
//}


