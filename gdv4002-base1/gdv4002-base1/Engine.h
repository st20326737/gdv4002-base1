#pragma once

#include "GraphicsCore.h"

#include "GUMemory.h"
#include "GUObject.h"
#include "GUClock.h"
#include "GameObject2D.h"
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include "texture_loader.h"

// Define function pointers for render and update functions
typedef void (*RenderFn)(GLFWwindow* window);
typedef void (*UpdateFn)(GLFWwindow* window, double tDelta);


//
// Top level engine functionality - setup, main loop and shutdown
//
int engineInit(const char* windowTitle, int initWidth = 1920, int initHeight = 1080, float initViewplaneWidth = 5.0f);
void engineMainLoop();
void engineShutdown();

//
// Event registration
//
void setKeyboardHandler(GLFWkeyfun newKeyboardHandler);
void setRenderFunction(RenderFn fn);
void setUpdateFunction(UpdateFn fn);

//
// Update / Query engine state
//

// Load a new texture image and return the OpenGL texture ID.  Store the texture internally in the engine for later use.
GLuint loadTexture(const char* texturePath, TextureProperties texProperties = TextureProperties());

// Provide properties to add a new game object.  This takes relevant properties and creates a new GameObject2D object in the game scene.
GameObject2D* addObject(
	const char* name,
	glm::vec2 initPosition = glm::vec2(0.0f, 0.0f),
	float initOrientation = 0.0f,
	glm::vec2 initSize = glm::vec2(1.0f, 1.0f),
	const char* texturePath = nullptr,
	TextureProperties texProperties = TextureProperties());

// Add an already existing object to the scene.  This overload of addObject can be used to provide subclasses of GameObject2D since addObject above creates GameObject2D instances directly.
GameObject2D* addObject(const char* name, GameObject2D* newObject);

// getObject returns the object with the *exact* key match
GameObject2D* getObject(const char* key);

void showAxisLines();
void hideAxisLines();
bool axisLinesVisible();
void setBackgroundColour(glm::vec4& newColour);
void setViewplaneWidth(float newWidth);
float getViewplaneWidth();
float getViewplaneHeight();


//
// Test functions
//
void listObjectCounts();
void listGameObjectKeys();