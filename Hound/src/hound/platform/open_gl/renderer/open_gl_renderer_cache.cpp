#include "hound/hnd_pch.h"
#include "open_gl_renderer_cache.h"
#include "hound/core/object/object_database.h"
#include "hound/core/object/shader/shader.h"
#include "hound/core/object/mesh/mesh.h"

renderer_cache::shader_id open_gl_renderer_cache::shader_create()
{
	const shader* shader_instance = object_database::get_instance()->create_object_instance<shader>();

	m_gl_shader_map_[shader_instance->get_object_id()].program_id = glCreateProgram();
	
	return shader_instance->get_object_id();
}

void open_gl_renderer_cache::shader_set_source(shader_id shader, shader_stage stage, const std::string& source)
{
	gl_shader_data& shader_data = m_gl_shader_map_[shader];
	
	gl_object_id gl_shader_id = 0;
	
	switch (stage)
	{
	case shader_stage::VERTEX:
		gl_shader_id = glCreateShader(GL_VERTEX_SHADER);
		break;
	case shader_stage::GEOMETRY:
		gl_shader_id = glCreateShader(GL_GEOMETRY_SHADER);
		break;
	case shader_stage::TESSELLATION:
		gl_shader_id = glCreateShader(GL_TESS_CONTROL_SHADER);
		break;
	case shader_stage::FRAGMENT:
		gl_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	case shader_stage::COMPUTE:
		gl_shader_id = glCreateShader(GL_COMPUTE_SHADER);
		break;
	}

	if(shader_data.program_stage_info[stage].stage_id != 0)
	{
		HND_CORE_LOG_WARN("SHADER::CREATION\r\n\tOverwrting shader source, deleting old shader");
		glDeleteShader(shader_data.program_stage_info[stage].stage_id);
	}
	shader_data.program_stage_info[stage].stage_id = gl_shader_id;
	shader_data.program_stage_info[stage].source = source;
}

const std::string& open_gl_renderer_cache::shader_get_source(shader_id shader, shader_stage stage)
{
	return m_gl_shader_map_[shader].program_stage_info[stage].source;
}

bool open_gl_renderer_cache::shader_finalize(shader_id shader)
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
		if(!compile_success)
		{
			char infoLog[512];
			glGetShaderInfoLog(pair.second.stage_id, 512, nullptr, infoLog);
			HND_CORE_LOG_ERROR("SHADER::COMPILE\tError compiling shader!\r\n\t- Output:", infoLog);
			success = false;
			break;
		}
	}

	//If not compiled, delete all shader data
	if(!success)
	{
		HND_CORE_LOG_ERROR("SHADER::COMPILE\tShader compilation failed, deleting shader");
		for (const auto& pair : shader_data.program_stage_info)
		{
			glDeleteShader(pair.second.stage_id);
		}
		glDeleteProgram(shader_data.program_id);
		return false;
	}

	//Attach all shader stages
	for (const auto& pair : shader_data.program_stage_info)
	{
		glAttachShader(shader_data.program_id, pair.second.stage_id);
	}

	//Link the shader program
	glLinkProgram(shader_data.program_id);

	//Check for linking errors
	GLint link_success;
	glGetProgramiv(shader_data.program_id, GL_LINK_STATUS, &link_success);
	if (!link_success) {
		char infoLog[512];
		glGetProgramInfoLog(shader_data.program_id, 512, NULL, infoLog);
		HND_CORE_LOG_ERROR("SHADER::LINKING\tError linking shader program!\r\n\t- Output:", infoLog);

		//Delete shader data if linking failed
		for (const auto& pair : shader_data.program_stage_info)
		{
			glDeleteShader(pair.second.stage_id);
		}
		glDeleteProgram(shader_data.program_id);
		return false;
	}

	return true;
}

gl_object_id open_gl_renderer_cache::get_shader_program_id(shader_id shader)
{
	return m_gl_shader_map_[shader].program_id;
}

renderer_cache::mesh_id open_gl_renderer_cache::mesh_create()
{
	const mesh* mesh_instance = object_database::get_instance()->create_object_instance<mesh>();

	gl_object_id id;
	glGenVertexArrays(1, &id);
	
	m_gl_mesh_map_[mesh_instance->get_object_id()].vertex_array_id = id;
	
	return mesh_instance->get_object_id();
}

renderer_cache::mesh_data_id open_gl_renderer_cache::mesh_add_data(mesh_id mesh, const mesh_data& data)
{
	if(!m_gl_mesh_map_.count(mesh))
	{
		HND_LOG_ERROR("No mesh found with id: ", static_cast<uint64_t>(mesh));
		return 0;
	}

	auto& mesh_object = m_gl_mesh_map_[mesh];
	auto& mesh_sub_data = mesh_object.sub_objects[mesh_object.data_count];
	
	mesh_sub_data.id = mesh_object.data_count;

	mesh_sub_data.vertices.resize(data.vertices.size());
	std::copy(data.vertices.begin(), data.vertices.end(), mesh_sub_data.vertices.begin());

	mesh_sub_data.indices.resize(data.indices.size());
	std::copy(data.indices.begin(), data.indices.end(), mesh_sub_data.indices.begin());
	
	glBindVertexArray(mesh_object.vertex_array_id);
	
	gl_object_id gl_vertex_buffer_id;
	glGenBuffers(1, &gl_vertex_buffer_id);
	
	mesh_sub_data.array_buffer_id = gl_vertex_buffer_id;
	
	glBindBuffer(GL_ARRAY_BUFFER, gl_vertex_buffer_id);
	
	glBufferData(GL_ARRAY_BUFFER, mesh_sub_data.vertices.size() * sizeof(mesh_data::vertex), &mesh_sub_data.vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(mesh_data::vertex), reinterpret_cast<void*>(offsetof(mesh_data::vertex, position)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(mesh_data::vertex), reinterpret_cast<void*>(offsetof(mesh_data::vertex, normal)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(mesh_data::vertex), reinterpret_cast<void*>(offsetof(mesh_data::vertex, uv)));
	glEnableVertexAttribArray(2);

	gl_object_id gl_element_buffer_id;
	glGenBuffers(1, &gl_element_buffer_id);
	
	mesh_sub_data.element_buffer_id = gl_element_buffer_id;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_element_buffer_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh_sub_data.indices.size() * sizeof(uint32_t), &mesh_sub_data.indices[0], GL_STATIC_DRAW);

	const mesh_data_id data_id = mesh_object.data_count;
	
	++mesh_object.data_count;
	
	return data_id;
}

bool open_gl_renderer_cache::mesh_override_data(mesh_id mesh, mesh_data_id data_id, const mesh_data& data)
{
	gl_mesh_object& mesh_data = m_gl_mesh_map_[mesh];

	if(data_id >= mesh_data.data_count)
	{
		HND_LOG_ERROR("Could not overwrite mesh data, data_id cannot be greater than the count");
		return false;
	}

	mesh_data.sub_objects[data_id].data = data;
	return true;
}

gl_object_id open_gl_renderer_cache::get_mesh_vertex_array_object(mesh_id mesh)
{
	return m_gl_mesh_map_[mesh].vertex_array_id;
}

open_gl_renderer_cache::open_gl_renderer_cache()
{
	s_instance_ = this;
}

open_gl_renderer_cache::~open_gl_renderer_cache()
{
}
