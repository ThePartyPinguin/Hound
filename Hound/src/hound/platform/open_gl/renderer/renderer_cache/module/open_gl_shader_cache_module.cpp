#include "hound/hnd_pch.h"
#include "open_gl_shader_cache_module.h"

#include "glad/glad.h"
#include "hound/core/object/object_database.h"
#include "hound/platform/open_gl/open_gl_core.h"
#include "hound/platform/open_gl/object/shader/open_gl_shader.h"

shader_cache_module::shader_id open_gl_shader_cache_module::shader_create()
{
	open_gl_shader* shader_instance = object_database::get_instance()->create_object_instance<open_gl_shader>();

	const gl_object_id program = glCreateProgram();

	m_gl_shader_map_[shader_instance->get_object_id()].gl_shader_program_id = program;
	shader_instance->set_shader_program_id(program);

	return shader_instance->get_object_id();
}

void open_gl_shader_cache_module::shader_set_source(shader_id shader, shader::stage stage, const std::string& source)
{
	gl_shader_data& shader_data = m_gl_shader_map_[shader];

	gl_object_id gl_shader_id = 0;

	switch (stage)
	{
	case shader::stage::VERTEX:
		gl_shader_id = glCreateShader(GL_VERTEX_SHADER);
		break;
	case shader::stage::GEOMETRY:
		gl_shader_id = glCreateShader(GL_GEOMETRY_SHADER);
		break;
	case shader::stage::TESSELLATION:
		gl_shader_id = glCreateShader(GL_TESS_CONTROL_SHADER);
		break;
	case shader::stage::FRAGMENT:
		gl_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	case shader::stage::COMPUTE:
		gl_shader_id = glCreateShader(GL_COMPUTE_SHADER);
		break;
	}

	if (shader_data.program_stage_info[stage].stage_id != 0)
	{
		HND_CORE_LOG_WARN("SHADER::CREATION\r\n\tOverwrting shader source, deleting old shader");
		glDeleteShader(shader_data.program_stage_info[stage].stage_id);
	}
	shader_data.program_stage_info[stage].stage_id = gl_shader_id;
	shader_data.program_stage_info[stage].source = source;
}

const std::string& open_gl_shader_cache_module::shader_get_source(shader_id shader, shader::stage stage)
{
	return m_gl_shader_map_[shader].program_stage_info[stage].source;
}

bool open_gl_shader_cache_module::shader_finalize(shader_id shader)
{
	const gl_shader_data& shader_data = m_gl_shader_map_[shader];

	bool success = true;
	for (const auto& pair : shader_data.program_stage_info)
	{
		//Set source
		const char* source_ptr = pair.second.source.c_str();
		glShaderSource(pair.second.stage_id, 1, &source_ptr, nullptr);

		//Compile
		glCompileShader(pair.second.stage_id);

		//Check for errors
		GLint compile_success;
		glGetShaderiv(pair.second.stage_id, GL_COMPILE_STATUS, &compile_success);
		if (!compile_success)
		{
			char infoLog[512];
			glGetShaderInfoLog(pair.second.stage_id, 512, nullptr, infoLog);
			HND_CORE_LOG_ERROR("SHADER::COMPILE\tError compiling shader!\r\n\t- Output:", infoLog);
			success = false;
			break;
		}
	}

	//If not compiled, delete all shader data
	if (!success)
	{
		HND_CORE_LOG_ERROR("SHADER::COMPILE\tShader compilation failed, deleting shader");
		for (const auto& pair : shader_data.program_stage_info)
		{
			glDeleteShader(pair.second.stage_id);
		}
		glDeleteProgram(shader_data.gl_shader_program_id);
		return false;
	}

	//Attach all shader stages
	for (const auto& pair : shader_data.program_stage_info)
	{
		glAttachShader(shader_data.gl_shader_program_id, pair.second.stage_id);
	}

	//Link the shader program
	glLinkProgram(shader_data.gl_shader_program_id);

	//Check for linking errors
	GLint link_success;
	glGetProgramiv(shader_data.gl_shader_program_id, GL_LINK_STATUS, &link_success);
	if (!link_success) {
		char infoLog[512];
		glGetProgramInfoLog(shader_data.gl_shader_program_id, 512, NULL, infoLog);
		HND_CORE_LOG_ERROR("SHADER::LINKING\tError linking shader program!\r\n\t- Output:", infoLog);

		//Delete shader data if linking failed
		for (const auto& pair : shader_data.program_stage_info)
		{
			glDeleteShader(pair.second.stage_id);
		}
		glDeleteProgram(shader_data.gl_shader_program_id);
		return false;
	}

	return true;
}

gl_object_id open_gl_shader_cache_module::get_shader_program_id(shader_id shader)
{
	return m_gl_shader_map_[shader].gl_shader_program_id;
}

open_gl_shader_cache_module::open_gl_shader_cache_module()
{
	
}

open_gl_shader_cache_module::~open_gl_shader_cache_module()
{
	for(auto& pair : m_gl_shader_map_)
	{
		glDeleteProgram(pair.second.gl_shader_program_id);
	}
}
