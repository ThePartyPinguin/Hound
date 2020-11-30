#pragma once
#include "hound/core/base.h"
#include <glad/glad.h>

typedef unsigned int gl_object_id;

#define HND_GL_MAJOR_VERSION 4
#define HND_GL_MINOR_VERSION 3
#define HND_GL_PROFILE GLFW_OPENGL_CORE_PROFILE

#if HND_DEBUG
#define HND_GL_CALL(x) x;\
		{\
			GLenum err;\
			while((err = glGetError()) != GL_NO_ERROR)\
			{\
				char message[128];\
				sprintf_s(message, 128, "%s %s:%o", #x, __FILE__, __LINE__);\
				glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR, 0, GL_DEBUG_SEVERITY_HIGH, 128, message);\
				__debugbreak();\
			}\
		}
#else
#define HND_GL_CALL(x) x;
#endif

