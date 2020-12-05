#ifndef ENGINE_H
#define ENGINE_H

#include <glad/glad.h>

#define GLFW_INCLUDE_ES2
#include <GLFW/glfw3.h>

#include "../linalg/mod.h"
#include "../logger/mod.h"
#include "../shader/mod.h"

GLFWwindow *Engine_CreateWindow(int width, int height);

GLuint Engine_Init(int width, int height);

#endif
