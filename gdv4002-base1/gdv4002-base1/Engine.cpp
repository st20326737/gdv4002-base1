#include "Engine.h"

#pragma region Engine variables

static GLFWwindow* window = nullptr;
static std::string windowTitleString;
static CoreStructures::GUClock* gameClock = nullptr;

static int windowWidth;
static int windowHeight;
static glm::vec2 viewplaneSize = glm::vec2(5.0f, 5.0f);
static float viewplaneAspect = 1.0f;

// Store single copy of textures in map - use path as key so don't load same texture more than once
static std::unordered_map<std::string, GLuint> textureLib;

// Store objects with a name key
static std::unordered_map<std::string, GameObject2D*> gameObjects;

// count number of instances of a given object name in gameObjects
static std::unordered_map<std::string, int> objectCount;

static bool _showAxisLines = true;

static glm::vec4 backgroundColour(0.0f, 0.0f, 0.0f, 1.0f);

static RenderFn overrideRenderFn = nullptr;
static UpdateFn overrideUpdateFn = nullptr;

#pragma endregion


// (private) function prototypes for default event handlers
void defaultRenderScene();
void defaultUpdateScene(double tDelta);
void defaultKeyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods);
void defaultResizeWindow(GLFWwindow* window, int width, int height);


using namespace std;
using namespace CoreStructures;


#pragma region Top level engine functionality

//
// Top level engine functionality - setup, main loop and shutdown
//

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

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glFrontFace(GL_CCW);

	// Initialise main game clock (starts by default)
	gameClock = new GUClock();

	return 0; // success
}


void engineMainLoop() {

	// Loop while program is not terminated.
	while (!glfwWindowShouldClose(window)) {

		// Update game clock and get time elapsed since last time round the loop!
		gameClock->tick();
		double tDelta = gameClock->gameTimeDelta();

		// Update game environment
		if (overrideUpdateFn)
			overrideUpdateFn(window, tDelta);
		else
			defaultUpdateScene(tDelta);

		// Render current frame
		defaultRenderScene();

		// Display newly rendered frame
		glfwSwapBuffers(window);

		// Poll events ie. user input (key presses, mouse events)
		glfwPollEvents();

		// (optional) Update window title to show current fps / spf
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

#pragma endregion


#pragma region Event Registration

//
// Event registration
//

 // Keyboard input callback
void setKeyboardHandler(GLFWkeyfun newKeyboardHandler) {

	glfwSetKeyCallback(window, newKeyboardHandler);
}

// Set render function - once set our own game render code will be used
void setRenderFunction(RenderFn fn) {

	overrideRenderFn = fn;
}

// Set update function - once set our own game update code will be used
void setUpdateFunction(UpdateFn fn) {

	overrideUpdateFn = fn;
}

#pragma endregion


#pragma region Update / query engine state

//
// Update / Query engine state
//

GLuint loadTexture(const char* texturePath, TextureProperties texProperties) {

	// Setup texture
	GLuint texture = 0;

	if (texturePath != nullptr) {

		// check texturePath is not already defined
		if (textureLib[texturePath] != NULL) {

			// texturePath seen before - get textureID from textureLib
			texture = textureLib[texturePath];
		}
		else {

			// texturePath not seen before so load texture
			texture = fiLoadTexture(texturePath, texProperties);
			if (texture > 0) {

				textureLib[texturePath] = texture;
			}
		}
	}

	return texture;
}


GameObject2D* addObject(const char* name, glm::vec2 initPosition, float initOrientation, glm::vec2 initSize, const char* texturePath, TextureProperties texProperties) {

	// Setup texture
	GLuint texture = loadTexture(texturePath, texProperties);

	// Create new object
	GameObject2D* newObject = new GameObject2D(initPosition, initOrientation, initSize, texture);

	return addObject(name, newObject);
	
}

GameObject2D* addObject(const char* name, GameObject2D* newObject) {

	if (newObject) {

		// If object created successfully setup string for new object 'key'
		string keyString;

		// Find out if object exists and set name key
		if (objectCount[name] == NULL) {

			// name not registered so first instance
			objectCount[name] = 1; // initialise key with a value of 1
			keyString = string(name);
		}
		else {

			objectCount[name] = objectCount[name] + 1; // pre-increment count against 'name'
			keyString = string(name) + to_string(objectCount[name]);
		}

		// Store object
		gameObjects[keyString] = newObject;
	}

	// return pointer to new object in case it's needed for further setup
	return newObject;
}


// getObject returns the object with the *exact* key match - return null if nothing matches
GameObject2D* getObject(const char* key) {

	return gameObjects[key];
}

// Return a collection of object where part of the object's key matches the 'key' parameter.  Value semantics is applied so when the caller eventually goes out of scope the collection's destructor will automatically free the internally allocated memory for the collection.
GameObjectCollection getObjectCollection(const char* key) {

	// Check at least one object has the key
	if (objectCount[key] == NULL) {

		// If key not present return empty collection (0, nullptr)
		return GameObjectCollection();
	}
	else {

		// Use count array to size collection
		GameObjectCollection collection = GameObjectCollection(objectCount[key]);

		// Iterate through object list and get those with keys that match
		int insertIndex = 0;
		for (auto iter = gameObjects.begin(); iter != gameObjects.end(); iter++) {

			if (iter->first.find(key) != std::string::npos) {

				collection.objectArray[insertIndex] = iter->second;
				insertIndex++;
			}
		}

		return collection;
	}
}


// Delete the game object with key 'key' and return true if successful, false otherwise.  This matches key *exactly*, it does not do a partial match
bool deleteObject(const char* key) {

	auto iter = gameObjects.find(key);

	if (iter != gameObjects.end()) {

		// Object to delete found - first store key string
		string objKey = iter->first;

		// ...the delete from gameObjects.
		gameObjects.erase(iter);

		// Now we need to string-match objKey to the objectCount array.
		// objectCount keys are a substring of gameObject keys that have numbers appended to differentiate.
		// When found we decrememt the count.  If it reaches zero erase the key from the count array
		for (auto countIter = objectCount.begin(); countIter != objectCount.end(); countIter++) {

			if (objKey.find(countIter->first) != std::string::npos) {

				countIter->second = countIter->second - 1; // decrement count

				if (countIter->second == 0) {

					objectCount.erase(countIter);
				}

				break;
			}
		}

		return true;
	}
	else {

		return false;
	}
}

// Delete the game object pointed to by objectPtr and return true if successful, false otherwise.  It is assumed 1 instance if each objectPtr exists in the object list maintained by the engine.  If this is not the case then the first instance of the pointer only is deleted.
bool deleteObject(GameObject2D* objectPtr) {

	bool objectErased = false;

	for (auto iter = gameObjects.begin(); iter != gameObjects.end(); iter++) {

		if (iter->second == objectPtr) {

			// Object to delete found - first store key string
			string objKey = iter->first;

			// ...the delete from gameObjects.
			gameObjects.erase(iter);
			objectErased = true;

			// Now we need to string-match objKey to the objectCount array.
			// objectCount keys are a substring of gameObject keys that have numbers appended to differentiate.
			// When found we decrememt the count.  If it reaches zero erase the key from the count array
			for (auto countIter = objectCount.begin(); countIter != objectCount.end(); countIter++) {

				if (objKey.find(countIter->first) != std::string::npos) {

					countIter->second = countIter->second - 1; // decrement count

					if (countIter->second == 0) {

						objectCount.erase(countIter);
					}

					break;
				}
			}

			break;
		}
	}

	return objectErased;
}

// Delete any object where the key partially matches 'key'.  Unlike deleteObject, this can be used to remove groups of like-named objects.  The function returns 0 if no objects matched and nothing was deleted, otherwise it returns the number of elements removed.
int deleteMatchingObjects(const char* key) {

	int eraseCount = 0;

	for (auto iter = gameObjects.begin(); iter != gameObjects.end();) {

		if (iter->first.find(key) != std::string::npos) {

			// Object to delete found - first store key string
			string objKey = iter->first;

			// ...the delete from gameObjects.
			iter = gameObjects.erase(iter);
			eraseCount++;

			// Now we need to string-match objKey to the objectCount array.
			// objectCount keys are a substring of gameObject keys that have numbers appended to differentiate.
			// When found we decrememt the count.  If it reaches zero erase the key from the count array
			for (auto countIter = objectCount.begin(); countIter != objectCount.end(); countIter++) {

				if (objKey.find(countIter->first) != std::string::npos) {

					countIter->second = countIter->second - 1; // decrement count

					if (countIter->second == 0) {

						objectCount.erase(countIter);
					}

					break;
				}
			}
		}
		else {

			iter++;
		}
	}

	return eraseCount;
}


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

void setViewplaneWidth(float newWidth) {

	viewplaneSize.x = newWidth;
	viewplaneSize.y = viewplaneSize.x * viewplaneAspect;
}

float getViewplaneWidth() {

	return viewplaneSize.x;
}

float getViewplaneHeight() {

	return viewplaneSize.y;
}

#pragma endregion


#pragma region Internal engine event Handlers

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

	glDisable(GL_DEPTH_TEST);

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

		auto objectIterator = gameObjects.begin();

		while (objectIterator != gameObjects.end()) {

			objectIterator->second->render();
			objectIterator++;
		}

	}
}

// Function called to update game objects in the scene
void defaultUpdateScene(double tDelta) {

	for (auto iter = gameObjects.begin(); iter != gameObjects.end(); iter++) {

		iter->second->update(tDelta);
	}
}

// Function to call when window resized
void defaultResizeWindow(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);		// Draw into entire window

	windowWidth = width;
	windowHeight = height;

	viewplaneAspect = (float)windowHeight / (float)windowWidth;
	viewplaneSize.y = viewplaneSize.x * viewplaneAspect;
}

void defaultKeyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check if a key is pressed
	if (action == GLFW_PRESS) {

		// check which key was pressed...
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			// If escape is pressed tell GLFW we want to close the window (and quit)
			glfwSetWindowShouldClose(window, true);
			break;

		default:
			{
			}
		}
	}
	// If not check a key has been released
	else if (action == GLFW_RELEASE) {

		// handle key release events
	}
}

#pragma endregion


#pragma region Test functions

void listObjectCounts() {

	printf("\nObject key counts...\n");
	for (auto iter = objectCount.begin(); iter != objectCount.end(); iter++) {

		printf("%s has count %d\n", iter->first.c_str(), iter->second);
	}
}

void listGameObjectKeys() {

	printf("\nIn-Game object keys...\n");
	for (auto iter = gameObjects.begin(); iter != gameObjects.end(); iter++) {

		printf("%s\n", iter->first.c_str());
	}
}

#pragma endregion


