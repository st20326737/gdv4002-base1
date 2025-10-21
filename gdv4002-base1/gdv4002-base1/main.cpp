#include "Engine.h"
#include "texture_loader.h"

// Function prototypes

void myRenderScene(GLFWwindow* window);
//void myKeyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods);

//void updateScene();

GLuint textureID;

int main(void) {

	int initResult = engineInit("GDV4002 - Applied Maths for Games", 1024, 1024);

	if (initResult != 0)
		return initResult; // exit if setup failed

	// test texture
	textureID = fiLoadTexture("Resources\\Textures\\bumblebee.png");
	printf("textureID = %d\n", textureID);

	setRenderFunction(myRenderScene);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glFrontFace(GL_CCW);

	engineMainLoop();

	engineShutdown();

	return 0;
}


void myRenderScene(GLFWwindow* window)
{
	// Render objects here...
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glBegin(GL_TRIANGLE_STRIP);

	glColor3f(1.0f, 1.0f, 1.0f);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-0.5f, -0.5f, 0.0f);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.5f, -0.5f, 0.0f);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-0.5f, 0.5f, 0.0f);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.5f, 0.5f, 0.0f);

	glEnd();
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


