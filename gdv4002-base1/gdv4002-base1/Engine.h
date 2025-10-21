#pragma once

#include "GraphicsCore.h"

#include "GUMemory.h"
#include "GUObject.h"
#include "GUClock.h"
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <iostream>
#include <string>
#include <glm/glm.hpp>

typedef void (*RenderFn)(GLFWwindow* window);


int engineInit(const char* windowTitle, int initWidth = 1920, int initHeight = 1080, float initViewplaneWidth = 5.0f);

void engineMainLoop();

void engineShutdown();

// Update / Query engine state
void showAxisLines();
void hideAxisLines();
bool axisLinesVisible();
void setBackgroundColour(glm::vec4& newColour);

// Event registration
void setKeyboardHandler(GLFWkeyfun newKeyboardHandler);

void setRenderFunction(RenderFn fn);