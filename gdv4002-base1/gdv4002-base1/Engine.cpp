#include "Engine.h"

// Engine variables
static GLFWwindow* window = nullptr;


// Function prototypes
void renderScene();
void updateScene();
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


	// Initialise glew
	glewInit();


	// Setup window's initial size
	resizeWindow(window, initWidth, initHeight);

	// Initialise scene - geometry and shaders etc
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // setup background colour to be black

	return 0; // success
}


void enterEngineMainLoop() {

	// Loop while program is not terminated.
	while (!glfwWindowShouldClose(window)) {

		updateScene();
		renderScene();				// Render into the current buffer
		glfwSwapBuffers(window);	// Displays what was just rendered (using double buffering).

		// Poll events (key presses, mouse events)
		// glfwWaitEvents();				// Use this if no animation.
		// glfwWaitEventsTimeout(1.0/60.0);	// Use this to animate at 60 frames/sec (timing is NOT reliable)
		glfwPollEvents();					// Use this version when animating as fast as possible
	}
}


// Event Registration

 // Keyboard input callback
void setKeyboardHandler(GLFWkeyfun newKeyboardHandler) {

	glfwSetKeyCallback(window, newKeyboardHandler);
}


// Internal engine event Handlers

void renderScene()
{
	// Clear the rendering window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render objects here...
}


// Function called to animate elements in the scene
void updateScene() {
}


// Function to call when window resized
void resizeWindow(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);		// Draw into entire window
}
