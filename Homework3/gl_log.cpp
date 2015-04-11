#include "gl_log.h"
#include <iostream>
#include <stdarg.h>
#include <time.h>

#define GL_LOG_FILE "gl.log"



bool restart_gl_log()
{
	FILE* file;
	fopen_s(&file, GL_LOG_FILE, "w");
	if (!file) {
		fprintf(stderr, "ERROR: FAILED TO OPEN %s FILE\n", GL_LOG_FILE);
		return false;
	}

	time_t now = time(NULL);
	char date[26];
	ctime_s(date, sizeof(char) * 26, &now);
	fprintf(file, "GL_LOG log. local time %s\n", date);
	fclose(file);
	return true;
}

bool gl_log(const char* message, ...)
{
	va_list argPointer;
	FILE* file;
	fopen_s(&file, GL_LOG_FILE, "a");
	if (!file) {
		fprintf(stderr, "ERROR: FAILED TO OPEN %s FILE\n", GL_LOG_FILE);
		return false;
	}

	va_start(argPointer, message);
	vfprintf(file, message, argPointer);
	va_end(argPointer);
	fclose(file);
	return true;
}

bool gl_log_error(const char* message, ...)
{
	va_list argPointer;
	FILE* file;
	fopen_s(&file, GL_LOG_FILE, "a");
	if (!file) {
		fprintf(stderr, "ERROR: FAILED TO OPEN %s FILE\n", GL_LOG_FILE);
		return false;
	}

	va_start(argPointer, message);
	vfprintf(file, message, argPointer);
	va_end(argPointer);
	va_start(argPointer, message);
	vfprintf(stderr, message, argPointer);
	va_end(argPointer);
	fclose(file);
	return true;
}

void log_gl_parameters()
{
	GLenum parameters[] = {
		GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
		GL_MAX_CUBE_MAP_TEXTURE_SIZE,
		GL_MAX_DRAW_BUFFERS,
		GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
		GL_MAX_TEXTURE_IMAGE_UNITS,
		GL_MAX_TEXTURE_SIZE,
		GL_MAX_VARYING_FLOATS,
		GL_MAX_VERTEX_ATTRIBS,
		GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
		GL_MAX_VERTEX_UNIFORM_COMPONENTS,
		GL_MAX_VIEWPORT_DIMS,
		GL_STEREO
	};

	const char* names[] = {
		"GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
		"GL_MAX_CUBE_MAP_TEXTURE_SIZE",
		"GL_MAX_DRAW_BUFFERS",
		"GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
		"GL_MAX_TEXTURE_IMAGE_UNITS",
		"GL_MAX_TEXTURE_SIZE",
		"GL_MAX_VARYING_FLOATS",
		"GL_MAX_VERTEX_ATTRIBS",
		"GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
		"GL_MAX_VERTEX_UNIFORM_COMPONENTS",
		"GL_MAX_VIEWPORT_DIMS",
		"GL_STEREO",
	};

	gl_log("GL Context Parameters:\n");
	char message[256];
	// integers - only works if the order is 0-10 integer return types
	for (int i = 0; i < 10; i++) {
		int v = 0;
		glGetIntegerv(parameters[i], &v);
		gl_log("%s %i\n", names[i], v);
	}

	int v[2];
	v[0] = v[1] = 0;
	glGetIntegerv(parameters[10], v);
	gl_log("%s %i %i\n", names[10], v[0], v[1]);
	unsigned char s = 0;
	glGetBooleanv(parameters[11], &s);
	gl_log("%s %u\n", parameters[11], (unsigned int)s);
	gl_log("-----------------------------\n");
}
