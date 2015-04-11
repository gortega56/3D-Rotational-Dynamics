#pragma once
#include <GL\glew.h>

bool restart_gl_log();
bool gl_log(const char* message, ...);
bool gl_log_error(const char* message, ...);
void log_gl_parameters();