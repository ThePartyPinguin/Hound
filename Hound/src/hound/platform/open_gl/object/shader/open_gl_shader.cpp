#include "hound/hnd_pch.h"
#include "open_gl_shader.h"
#include "glad/glad.h"

void open_gl_shader::use()
{
	HND_GL_CALL(glUseProgram, m_program_id_);
}

void open_gl_shader::set_uniform_float(const char* u_name, float value)
{
	const GLint location = HND_GL_CALL(glGetUniformLocation, m_program_id_, u_name);
	HND_GL_CALL(glUniform1f, location, value);
}

open_gl_shader::~open_gl_shader()
{
	HND_GL_CALL(glDeleteProgram, m_program_id_);
}
