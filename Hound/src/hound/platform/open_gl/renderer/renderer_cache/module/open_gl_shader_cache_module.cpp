#include "hound/hnd_pch.h"
#include "open_gl_shader_cache_module.h"

#include "hound/core/object/shader/shader.h"
#include "glad/glad.h"
#include "hound/core/object/object_database.h"
#include "hound/core/os/os.h"
#include "hound/platform/open_gl/open_gl_core.h"
#include "hound/core/io/file/file_handle.h"
#include "hound/file/shader/raw/raw_shader.h"
#include "hound/file/shader/raw/raw_shader_parser.h"

RENDER_CACHE_CLASS_IMPL(shader_cache_module, shader)
RENDER_CACHE_CREATE_FUNC_IMPL_NAMED_P1(shader, shader_cache_module, by_name, const char*, name)
RENDER_CACHE_CREATE_FUNC_IMPL_NAMED_P1(shader, shader_cache_module, from_absolute_path, const char*, absolute_path)
RENDER_CACHE_CREATE_FUNC_IMPL_NAMED_P1(shader, shader_cache_module, from_asset_path, const char*, asset_path)

void open_gl_shader_cache_module::on_create_instance_by_name(shader* instance, const char* name)
{
	const gl_object_id program = HND_GL_CALL(glCreateProgram);

	gl_shader_data& data = m_gl_shader_map_[instance->get_object_id()];

	data.name = std::string(name);
	data.gl_shader_program_id = program;
	data.shader_object = instance;
	data.name = std::string(name);
}

void open_gl_shader_cache_module::on_create_instance_from_absolute_path(shader* instance, const char* absolute_path)
{
	file_handle* handle = os::get_instance()->file_handle_from_absolute_path(absolute_path);

	const bool exists = handle->exists();

	raw_shader raw;
	if(exists)
	{
		size_t size;
		bool success;
		char* file_data = handle->read(size, success);

		if(success)
		{
			raw_shader_parser::parse(raw, file_data, size);
		}
		else
		{
			HND_CORE_LOG_WARN("Could not read source file at:", absolute_path);
			delete handle;
			return;
		}
		delete handle;
	}
	else
	{
		HND_CORE_LOG_WARN("Could not read shader source file at:", absolute_path);
		delete handle;
		return;
	}

	shader_id id = instance->get_object_id();

	gl_shader_data& data = m_gl_shader_map_[id];
	data.gl_shader_program_id = HND_GL_CALL(glCreateProgram);
	data.shader_object = instance;
	data.name = std::string(raw.shader_name);

	for (const auto& stage : raw.shader_stages)
	{
		shader_set_source(id, stage.second.shader_type, stage.second.shader_source);
	}

	const bool success = shader_finalize(id);

	if (!success)
	{
		HND_CORE_LOG_WARN("Could not create shader:", data.name);
	}	
}

void open_gl_shader_cache_module::on_create_instance_from_asset_path(shader* instance, const char* asset_path)
{
	file_handle* handle = os::get_instance()->file_handle_from_asset_path(asset_path);

	const bool exists = handle->exists();

	raw_shader raw;
	if (exists)
	{
		size_t size;
		bool success;
		char* file_data = handle->read(size, success);

		if (success)
		{
			raw_shader_parser::parse(raw, file_data, size);
		}
		else
		{
			HND_CORE_LOG_WARN("Could not read source file at:", asset_path);
			delete handle;
			return;
		}
		delete handle;
	}
	else
	{
		HND_CORE_LOG_WARN("Could not read shader source file at:", asset_path);
		delete handle;
		return;
	}

	shader_id id = instance->get_object_id();

	gl_shader_data& data = m_gl_shader_map_[id];
	data.gl_shader_program_id = HND_GL_CALL(glCreateProgram);
	data.shader_object = instance;
	data.name = std::string(raw.shader_name);

	for (const auto& stage : raw.shader_stages)
	{
		shader_set_source(id, stage.second.shader_type, stage.second.shader_source);
	}

	const bool success = shader_finalize(id);

	if (!success)
	{
		HND_CORE_LOG_WARN("Could not create shader:", data.name);
	}
}

shader* open_gl_shader_cache_module::get_shader_object(shader_id shader)
{
	if(!m_gl_shader_map_.count(shader))
	{
		HND_CORE_LOG_WARN("Could not find shader, returning nullptr");
		return nullptr;
	}

	return m_gl_shader_map_[shader].shader_object;
}

void open_gl_shader_cache_module::shader_set_source(shader_id shader, shader_stage stage, const std::string& source)
{
	gl_shader_data& shader_data = m_gl_shader_map_[shader];

	gl_object_id gl_shader_id = 0;

	switch (stage)
	{
	case SHADER_STAGE_VERTEX:
		gl_shader_id = HND_GL_CALL(glCreateShader, GL_VERTEX_SHADER);
		break;
	case SHADER_STAGE_GEOMETRY:
		gl_shader_id = HND_GL_CALL(glCreateShader, GL_GEOMETRY_SHADER);
		break;
	case SHADER_STAGE_TESSELLATION:
		gl_shader_id = HND_GL_CALL(glCreateShader, GL_TESS_CONTROL_SHADER);
		break;
	case SHADER_STAGE_FRAGMENT:
		gl_shader_id = HND_GL_CALL(glCreateShader, GL_FRAGMENT_SHADER);
		break;
	case SHADER_STAGE_COMPUTE:
		gl_shader_id = HND_GL_CALL(glCreateShader, GL_COMPUTE_SHADER);
		break;
	}

	if (shader_data.program_stage_info[stage].gl_shader_stage_id != 0)
	{
		HND_CORE_LOG_WARN("SHADER::CREATION\r\n\tOverwrting shader source, deleting old shader");
		HND_GL_CALL(glDeleteShader, shader_data.program_stage_info[stage].gl_shader_stage_id);
	}
	shader_data.program_stage_info[stage].gl_shader_stage_id = gl_shader_id;
	shader_data.program_stage_info[stage].source = source;
}

const std::string& open_gl_shader_cache_module::shader_get_source(shader_id shader, shader_stage stage)
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
		const GLsizei length = strlen(source_ptr);
		
		HND_GL_CALL(glShaderSource, pair.second.gl_shader_stage_id, 1, &source_ptr, &length);

		//Compile
		HND_GL_CALL(glCompileShader, pair.second.gl_shader_stage_id);

		//Check for errors
		GLint compile_success;
		HND_GL_CALL(glGetShaderiv, pair.second.gl_shader_stage_id, GL_COMPILE_STATUS, &compile_success);
		if (!compile_success)
		{
			char infoLog[512];
			HND_GL_CALL(glGetShaderInfoLog, pair.second.gl_shader_stage_id, 512, nullptr, infoLog);
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
			HND_GL_CALL(glDeleteShader, pair.second.gl_shader_stage_id);
		}
		HND_GL_CALL(glDeleteProgram, shader_data.gl_shader_program_id);
		return false;
	}

	//Attach all shader stages
	for (const auto& pair : shader_data.program_stage_info)
	{
		HND_GL_CALL(glAttachShader, shader_data.gl_shader_program_id, pair.second.gl_shader_stage_id);
	}

	//Link the shader program
	HND_GL_CALL(glLinkProgram, shader_data.gl_shader_program_id);

	//Check for linking errors
	GLint link_success;
	HND_GL_CALL(glGetProgramiv, shader_data.gl_shader_program_id, GL_LINK_STATUS, &link_success);
	if (!link_success) {

		GLint logSize;
		glGetProgramiv(shader_data.gl_shader_program_id, GL_INFO_LOG_LENGTH, &logSize);
		char* infoLog = new char[logSize];
		// glGetProgramInfoLog(shader_data.gl_shader_program_id, logSize, &logSize, infoLog);
		HND_GL_CALL(glGetProgramInfoLog, shader_data.gl_shader_program_id, logSize, &logSize, infoLog);
		HND_CORE_LOG_ERROR("SHADER::LINKING\tError linking shader program!\r\n\t- Output:", infoLog);

		//Delete shader data if linking failed
		for (const auto& pair : shader_data.program_stage_info)
		{
			HND_GL_CALL(glDeleteShader, pair.second.gl_shader_stage_id);
		}
		HND_GL_CALL(glDeleteProgram, shader_data.gl_shader_program_id);
		return false;
	}

	return true;
}

gl_object_id open_gl_shader_cache_module::get_shader_program_id(shader_id shader)
{
	return m_gl_shader_map_[shader].gl_shader_program_id;
}

const shader_cache_module::shader_data& open_gl_shader_cache_module::get_shader_data(shader_id shader)
{
	return m_gl_shader_map_[shader];
}

shader_id open_gl_shader_cache_module::get_standard_screen_shader()
{
	return m_standard_screen_shader_;
}

void open_gl_shader_cache_module::use_shader(shader_id shader)
{
	glUseProgram(m_gl_shader_map_[shader].gl_shader_program_id);
}

void open_gl_shader_cache_module::set_uniform_float(shader_id shader, const char* name, float value)
{
	const gl_object_id program = m_gl_shader_map_[shader].gl_shader_program_id;
	const GLint location = glGetUniformLocation(program, name);
	glUniform1f(location, value);
}

void open_gl_shader_cache_module::set_uniform_mat4_f(shader_id shader, const char* name, const mat4_f& value)
{
	const gl_object_id program = m_gl_shader_map_[shader].gl_shader_program_id;
	const GLint location = glGetUniformLocation(program, name);
	glUniformMatrix4fv(location, 1, GL_FALSE, value.get_value_ptr());
}

void open_gl_shader_cache_module::init()
{
	create_standard_screen_shader();
}

open_gl_shader_cache_module::open_gl_shader_cache_module()
{
	s_instance_ = this;
}

open_gl_shader_cache_module::~open_gl_shader_cache_module()
{
	for(auto& pair : m_gl_shader_map_)
	{
		HND_GL_CALL(glDeleteProgram, pair.second.gl_shader_program_id);
	}
}

void open_gl_shader_cache_module::create_standard_screen_shader()
{
	const char* vert_shader_src = R"(
	#version 430 core
	layout(location = 0) in vec2 aPos;
	layout(location = 2) in vec2 aTexCoords;

	layout (std140, binding = 0) uniform Matrices
	{
		mat4 projection;
		mat4 view;
	};
	
	out vec2 TexCoords;

	void main()
	{
		gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
		TexCoords = aTexCoords;
	})";

	const char* frag_shader_src = R"(
	#version 430 core
	out vec4 FragColor;

	in vec3 color;
	in vec2 TexCoords;

	uniform sampler2D screenTexture;

	void main()
	{ 
	    FragColor = texture(screenTexture, TexCoords);
	})";

	m_standard_screen_shader_ = shader::create_by_name("Standard screen shader\0")->get_object_id();
	shader_set_source(m_standard_screen_shader_, shader_stage::SHADER_STAGE_VERTEX, vert_shader_src);
	shader_set_source(m_standard_screen_shader_, shader_stage::SHADER_STAGE_FRAGMENT, frag_shader_src);
	shader_finalize(m_standard_screen_shader_);
}
