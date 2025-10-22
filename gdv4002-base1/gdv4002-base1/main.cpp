#include "Engine.h"


// Function prototypes

void myRenderScene(GLFWwindow* window);
void myUpdateScene(GLFWwindow* window, double tDelta);

//void myKeyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods);

int main(void) {

	int initResult = engineInit("GDV4002 - Applied Maths for Games", 1024, 1024);

	if (initResult != 0)
		return initResult; // exit if setup failed

	// Test object setup
	GameObject2D* player1 = addObject("player", glm::vec2(-2.0f, 0.0f), 0.0f, glm::vec2(0.5f, 0.5f), "Resources\\Textures\\bumblebee.png");
	GameObject2D* player2 = addObject("player", glm::vec2(2.0f, 1.0f), 0.0f, glm::vec2(0.25f, 0.25f), "Resources\\Textures\\bumblebee.png");

	listObjectCounts();
	listGameObjectKeys();

	setUpdateFunction(myUpdateScene);

	engineMainLoop();

	engineShutdown();

	return 0;
}


void myRenderScene(GLFWwindow* window)
{
}

void myUpdateScene(GLFWwindow* window, double tDelta) {

	static float theta = 0.0f; // angle - in radians

	const float pi = 3.141593f;
	const float thetaVelocity = (pi / 180.0f) * 90.0f; // angle change per second (90 degrees)

	GameObject2D* player1 = getObject("player");

	player1->position.x = cosf(theta);
	player1->position.y = sinf(theta);
	

	theta += thetaVelocity * tDelta;
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


