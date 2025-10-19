#pragma once

#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <Windows.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <iostream>
#include <glm/glm.hpp>


int engineInit(const char* windowTitle, int initWidth = 1920, int initHeight = 1080);

void enterEngineMainLoop();

// Event registration
void setKeyboardHandler(GLFWkeyfun newKeyboardHandler);
