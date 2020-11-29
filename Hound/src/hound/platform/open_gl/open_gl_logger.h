#pragma once
#include "glad/glad.h"

class open_gl_logger
{
public:
	static void init();

private:
	static void gl_error_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

	static void get_gl_error_source(char* log_string, GLenum source);
	static void get_gl_error_type(char* log_string, GLenum type);
};

