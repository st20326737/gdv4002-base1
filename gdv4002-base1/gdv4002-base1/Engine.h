#pragma once

#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GUMemory.h"
#include "GUObject.h"
#include "GUClock.h"
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <iostream>
#include <string>
#include <glm/glm.hpp>


int engineInit(const char* windowTitle, int initWidth = 1920, int initHeight = 1080);

void engineMainLoop();

void engineShutdown();


// Event registration
void setKeyboardHandler(GLFWkeyfun newKeyboardHandler);
