#ifndef ENGINE_H
#define ENGINE_H

#include <glad/glad.h>

#define GLFW_INCLUDE_ES2
#include <GLFW/glfw3.h>

#include "../logger/mod.h"
#include "../shader/mod.h"

void process_input(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

GLFWwindow *init_window(int width, int height);

GLint Engine_Init();

#endif
