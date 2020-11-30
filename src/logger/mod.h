#ifndef LOGGER_H
#define LOGGER_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <glad/glad.h>

#define GLFW_INCLUDE_ES2
#include <GLFW/glfw3.h>

typedef enum { TRACE, DEBUG, INFO, WARNING, ERROR, FATAL } Level;

int Logger_CheckGLErrors(const char *message);

void Log(Level level, const char *message);
void Logger_GetLogInfo(const char *message, GLuint obj, int type);

#endif
