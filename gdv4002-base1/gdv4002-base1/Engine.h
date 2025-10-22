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


typedef void (*RenderFn)(GLFWwindow* window);
typedef void (*UpdateFn)(GLFWwindow* window, double tDelta);


// Engine setup, main loop and shutdown
int engineInit(const char* windowTitle, int initWidth = 1920, int initHeight = 1080, float initViewplaneWidth = 5.0f);
void engineMainLoop();
void engineShutdown();


// Update / Query engine state
GameObject2D* addObject(const char* name, glm::vec2 initPosition, float initOrientation, glm::vec2 initSize = glm::vec2(1.0f, 1.0f), const char* texturePath = nullptr, TextureProperties texProperties = TextureProperties());

// getObject returns the object with the *exact* key match
GameObject2D* getObject(const char* key);

void showAxisLines();
void hideAxisLines();
bool axisLinesVisible();
void setBackgroundColour(glm::vec4& newColour);

// Event registration
void setKeyboardHandler(GLFWkeyfun newKeyboardHandler);
void setRenderFunction(RenderFn fn);
void setUpdateFunction(UpdateFn fn);


// Test functions
void listObjectCounts();
void listGameObjectKeys();