#pragma once
#include <glad/glad.h>

#define HND_GL_BINDING_SHADER_CAMERA_MATRICES 0

struct gl_binding_constant
{
	const static GLint camera_matrices = HND_GL_BINDING_SHADER_CAMERA_MATRICES;
};
