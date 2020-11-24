#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>

#define GLFW_INCLUDE_ES2
#include <GLFW/glfw3.h>

int Logger_CheckGLErrors(const char *message);
void Logger_GetLogInfo(const char *message, GLuint obj, int type);

#endif
