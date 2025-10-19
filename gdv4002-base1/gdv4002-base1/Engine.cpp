#include "Engine.h"

// Engine variables
static GLFWwindow* window = nullptr;


// (private) function prototypes
void defaultRenderScene();
void defaultUpdateScene();
void defaultKeyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods);
void resizeWindow(GLFWwindow* window, int width, int height);


using namespace std;


int engineInit(const char* windowTitle, int initWidth, int initHeight) {

	// Initialise glfw and setup window
	glfwInit();

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GLFW_TRUE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);

	window = glfwCreateWindow(initWidth, initHeight, windowTitle, NULL, NULL);

	// Check window was created successfully
	if (window == NULL)
	{
		cout << "Failed to create GLFW window!\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Set callback functions to handle different events
	glfwSetFramebufferSizeCallback(window, resizeWindow);
	glfwSetKeyCallback(window, defaultKeyboardHandler);

	// Initialise glew
	glewInit();


	// Setup window's initial size
	resizeWindow(window, initWidth, initHeight);

	// Initialise scene - geometry and shaders etc
	glClearColor(0.0f, 0.0f, 1.0f, 0.0f); // setup background colour to be black

	return 0; // success
}


void engineMainLoop() {

	// Loop while program is not terminated.
	while (!glfwWindowShouldClose(window)) {

		// Update game environment
		defaultUpdateScene();

		// Render current frame
		defaultRenderScene();

		// Display newly rendered frame
		glfwSwapBuffers(window);

		// Poll events ie. user input (key presses, mouse events)
		glfwPollEvents();
	}
}


// Event Registration

 // Keyboard input callback
void setKeyboardHandler(GLFWkeyfun newKeyboardHandler) {

	glfwSetKeyCallback(window, newKeyboardHandler);
}


// Internal engine event Handlers

void defaultRenderScene()
{
	// Clear the rendering window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render objects here...

	
}

// Function called to animate elements in the scene
void defaultUpdateScene() {

}


// Function to call when window resized
void resizeWindow(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);		// Draw into entire window
}


void defaultKeyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods)
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