#include "Engine.h"

#pragma region Engine variables

static GLFWwindow* window = nullptr;
static std::string windowTitleString;
static CoreStructures::GUClock* gameClock = nullptr;

static int windowWidth;
static int windowHeight;
static glm::vec2 viewplaneSize = glm::vec2(5.0f, 5.0f);
static float viewplaneAspect = 1.0f;

static bool _showAxisLines = true;

static glm::vec4 backgroundColour(0.0f, 0.0f, 0.0f, 1.0f);

static RenderFn overrideRenderFn = nullptr;

#pragma endregion


// (private) function prototypes
void defaultRenderScene();
void defaultUpdateScene(double tDelta);
void defaultKeyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods);
void defaultResizeWindow(GLFWwindow* window, int width, int height);


using namespace std;
using namespace CoreStructures;


int engineInit(const char* windowTitle, int initWidth, int initHeight, float initViewplaneWidth) {

	// Initialise glfw and setup window
	glfwInit();

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GLFW_TRUE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);

	window = glfwCreateWindow(initWidth, initHeight, windowTitle, NULL, NULL);

	// Check window was created successfully
	if (window == NULL)
	{
		cout << "Failed to create GLFW window!\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	windowTitleString = std::string(windowTitle);
	
	windowWidth = initWidth;
	windowHeight = initHeight;
	viewplaneAspect = (float)windowHeight / (float)windowWidth;
	viewplaneSize.x = initViewplaneWidth;
	viewplaneSize.y = viewplaneSize.x * viewplaneAspect;

	// Set callback functions to handle different events
	glfwSetFramebufferSizeCallback(window, defaultResizeWindow);
	glfwSetKeyCallback(window, defaultKeyboardHandler);

	// Initialise glew
	glewInit();

	// Setup window's initial size
	defaultResizeWindow(window, initWidth, initHeight);

	// Initialise scene - geometry and shaders etc
	glClearColor(0.0f, 0.0f, 1.0f, 0.0f); // setup background colour to be black

	// Initialise main game clock (starts by default)
	gameClock = new GUClock();

	return 0; // success
}


void engineMainLoop() {

	// Loop while program is not terminated.
	while (!glfwWindowShouldClose(window)) {

		gameClock->tick();
		double tDelta = gameClock->gameTimeDelta();

		// Update game environment
		defaultUpdateScene(tDelta);

		// Render current frame
		defaultRenderScene();

		// Display newly rendered frame
		glfwSwapBuffers(window);

		// Poll events ie. user input (key presses, mouse events)
		glfwPollEvents();

		char timingString[256];
		sprintf_s(timingString, 256, "%s: Average fps: %.0f; Average spf: %f", windowTitleString.c_str(), gameClock->averageFPS(), gameClock->averageSPF() / 1000.0f);
		glfwSetWindowTitle(window, timingString);
	}
}

void engineShutdown() {

	// Ready to exit - report timing data from loop epoch
	if (gameClock) {

		gameClock->stop();
		gameClock->reportTimingData();
	}

	glfwTerminate();
}


// Event Registration

 // Keyboard input callback
void setKeyboardHandler(GLFWkeyfun newKeyboardHandler) {

	glfwSetKeyCallback(window, newKeyboardHandler);
}

void setRenderFunction(RenderFn fn) {

	overrideRenderFn = fn;
}


// Internal engine event Handlers

void defaultRenderScene()
{
	glClearColor(backgroundColour.r,
		backgroundColour.g,
		backgroundColour.b,
		backgroundColour.a);

	// Clear the rendering window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-viewplaneSize.x / 2.0f, viewplaneSize.x / 2.0f, -viewplaneSize.y / 2.0f, viewplaneSize.y / 2.0f, -1.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Render axis lines if flag set
	if (_showAxisLines) {

		glColor3f(0.5f, 0.5f, 0.5f);
		
		glBegin(GL_LINES);

		glVertex2f(0.0f, -viewplaneSize.y);
		glVertex2f(0.0f, viewplaneSize.y);

		glVertex2f(-viewplaneSize.x, 0.0f);
		glVertex2f(viewplaneSize.x, 0.0f);

		glEnd();
	}

	// call render override if set...
	if (overrideRenderFn != nullptr) {

		overrideRenderFn(window);
	} 
	else {

		// ...if not set render default scene

	}
}

// Function called to animate elements in the scene
void defaultUpdateScene(double tDelta) {

}


// Function to call when window resized
void defaultResizeWindow(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);		// Draw into entire window

	windowWidth = width;
	windowHeight = height;

	viewplaneAspect = (float)windowHeight / (float)windowWidth;
	viewplaneSize.x = 5.0f;
	viewplaneSize.y = viewplaneSize.x * viewplaneAspect;
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


// Update / query engine state

void showAxisLines() {

	_showAxisLines = true;
}

void hideAxisLines() {

	_showAxisLines = false;
}

bool axisLinesVisible() {

	return _showAxisLines;
}

void setBackgroundColour(glm::vec4& newColour) {

	backgroundColour = newColour;
}