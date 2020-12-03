#include "hound/hnd_pch.h"
#include "open_gl_renderer_cache.h"
#include "hound/core/object/object_database.h"
#include "hound/core/object/shader/shader.h"
#include "hound/core/object/texture/texture.h"
#include "hound/core/object/mesh/mesh.h"
#include "hound/core/rendering/renderer_cache/module/texture/texture_cache_module.h"
#include "hound/platform/open_gl/renderer/renderer_cache/module/open_gl_texture_cache_module.h"
#include "hound/platform/open_gl/object/shader/open_gl_shader.h"
#include "hound/platform/open_gl/renderer/render_target/open_gl_frame_buffer.h"

renderer_cache::shader_id open_gl_renderer_cache::shader_create()
{
	open_gl_shader* shader_instance = object_database::get_instance()->create_object_instance<open_gl_shader>();

	const gl_object_id program = glCreateProgram();
	
	m_gl_shader_map_[shader_instance->get_object_id()].gl_shader_program_id = program;
	shader_instance->set_shader_program_id(program);
	
	return shader_instance->get_object_id();
}

void open_gl_renderer_cache::shader_set_source(shader_id shader, shader::stage stage, const std::string& source)
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

	if(shader_data.program_stage_info[stage].stage_id != 0)
	{
		HND_CORE_LOG_WARN("SHADER::CREATION\r\n\tOverwrting shader source, deleting old shader");
		glDeleteShader(shader_data.program_stage_info[stage].stage_id);
	}
	shader_data.program_stage_info[stage].stage_id = gl_shader_id;
	shader_data.program_stage_info[stage].source = source;
}

const std::string& open_gl_renderer_cache::shader_get_source(shader_id shader, shader::stage stage)
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

gl_object_id open_gl_renderer_cache::get_shader_program_id(shader_id shader)
{
	return m_gl_shader_map_[shader].gl_shader_program_id;
}

renderer_cache::mesh_id open_gl_renderer_cache::mesh_create()
{
	const mesh* mesh_instance = object_database::get_instance()->create_object_instance<mesh>();

	gl_object_id id;
	glGenVertexArrays(1, &id);
	
	m_gl_mesh_map_[mesh_instance->get_object_id()].gl_vertex_array_id = id;
	
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
	
	glBindVertexArray(mesh_object.gl_vertex_array_id);
	
	gl_object_id gl_vertex_buffer_id;
	glGenBuffers(1, &gl_vertex_buffer_id);
	
	mesh_sub_data.gl_array_buffer_id = gl_vertex_buffer_id;
	
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
	
	mesh_sub_data.gl_element_buffer_id = gl_element_buffer_id;

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
	return m_gl_mesh_map_[mesh].gl_vertex_array_id;
}

renderer_cache::texture_id open_gl_renderer_cache::texture_create_2d()
{
	texture* texture_instance = object_database::get_instance()->create_object_instance<texture>();

	gl_object_id gl_texture_object;
	glGenTextures(1, &gl_texture_object);

	gl_texture_data& texture = m_gl_texture_map_[texture_instance->get_object_id()];
	texture.gl_texture_object_id = gl_texture_object;
	texture.type = TEXTURE_TYPE_2D;

	return texture_instance->get_object_id();
}

void open_gl_renderer_cache::texture_set_2d_size(texture_id texture, const vec2_i& size)
{
	if (!m_gl_texture_map_.count(texture))
	{
		HND_CORE_LOG_WARN("RENDERER_CACHE\tTexture does not exist!");
		return;
	}
	
	gl_texture_data& data = m_gl_texture_map_[texture];

	if(data.type != TEXTURE_TYPE_2D)
	{
		HND_CORE_LOG_WARN("RENDERER_CACHE\tTexture is not a 2d texture, could not set size!");
		return;
	}

	data.size = size;
	
	glBindTexture(GL_TEXTURE_2D, data.gl_texture_object_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, data.size.get_x(), data.size.get_y(), 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
}

void open_gl_renderer_cache::texture_set_2d_filter_mode(texture_id texture, texture_filter_mode mode)
{
	if (!m_gl_texture_map_.count(texture))
	{
		HND_CORE_LOG_WARN("RENDERER_CACHE\tTexture does not exist!");
		return;
	}

	gl_texture_data& data = m_gl_texture_map_[texture];

	if (data.type != TEXTURE_TYPE_2D)
	{
		HND_CORE_LOG_WARN("RENDERER_CACHE\tTexture is not a 2d texture, could not set filter mode!");
		return;
	}
		
	glBindTexture(GL_TEXTURE_2D, data.gl_texture_object_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture_get_gl_filter_mode(data.filter_mode));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture_get_gl_filter_mode(data.filter_mode));

	glBindTexture(GL_TEXTURE_2D, 0);
}

void open_gl_renderer_cache::texture_set_2d_wrap_mode(texture_id texture, texture_wrap_mode h, texture_wrap_mode v)
{
	if (!m_gl_texture_map_.count(texture))
	{
		HND_CORE_LOG_WARN("RENDERER_CACHE\tTexture does not exist!");
		return;
	}

	gl_texture_data& data = m_gl_texture_map_[texture];

	if (data.type != TEXTURE_TYPE_2D)
	{
		HND_CORE_LOG_WARN("RENDERER_CACHE\tTexture is not a 2d texture, could not set wrap mode!");
		return;
	}
	data.wrap_horizontal = h;
	data.wrap_vertical = v;
	
	const gl_object_id texture_object = data.gl_texture_object_id;
	glBindTexture(GL_TEXTURE_2D, texture_object);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture_get_gl_wrap_mode(data.wrap_horizontal));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture_get_gl_wrap_mode(data.wrap_vertical));

	glBindTexture(GL_TEXTURE_2D, 0);
}

void open_gl_renderer_cache::texture_set_2d_data(texture_id texture, unsigned char* raw_data)
{
	gl_texture_data& texture_data = m_gl_texture_map_[texture];

	if (texture_data.type != TEXTURE_TYPE_2D)
	{
		HND_CORE_LOG_WARN("RENDERER_CACHE\tTexture is not a 2d texture, could not set size!");
		return;
	}

	glBindTexture(GL_TEXTURE_2D, texture_data.gl_texture_object_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_data.size.get_x(), texture_data.size.get_y(), 0, GL_RGB, GL_UNSIGNED_BYTE, raw_data);

	glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned char* open_gl_renderer_cache::get_texture_2d_data(texture_id texture, vec2_i& size)
{
	if (!m_gl_texture_map_.count(texture))
	{
		HND_CORE_LOG_WARN("RENDERER_CACHE\tTexture does not exist!");
		return nullptr;
	}

	gl_texture_data& data = m_gl_texture_map_[texture];

	if (data.type != TEXTURE_TYPE_2D)
	{
		HND_CORE_LOG_WARN("RENDERER_CACHE\tTexture is not a 2d texture, could not set wrap mode!");
		return nullptr;
	}

	size = data.size;

	const size_t buffer_size = data.size.get_x() * data.size.get_y();

	auto* raw_texture_data = new unsigned char[buffer_size];

	glBindTexture(GL_TEXTURE_2D, data.gl_texture_object_id);
	glGetnTexImage(TEXTURE_TYPE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer_size, raw_texture_data);

	glBindTexture(GL_TEXTURE_2D, 0);
	return raw_texture_data;
}

void open_gl_renderer_cache::texture_generate_2d_mip_map(texture_id texture)
{
	if (!m_gl_texture_map_.count(texture))
	{
		HND_CORE_LOG_WARN("RENDERER_CACHE\tTexture does not exist!");
		return;
	}

	gl_texture_data& data = m_gl_texture_map_[texture];

	if (data.type != TEXTURE_TYPE_2D)
	{
		HND_CORE_LOG_WARN("RENDERER_CACHE\tTexture is not a 2d texture, could not set wrap mode!");
		return;
	}

	if (data.has_mip_map)
	{
		HND_CORE_LOG_WARN("RENDERER_CACHE\tTexture already has mipmaps! Not generating again");
		return;
	}
	
	glBindTexture(GL_TEXTURE_2D, data.gl_texture_object_id);
	glGenerateMipmap(GL_TEXTURE_2D);
	data.has_mip_map = true;
	glBindTexture(GL_TEXTURE_2D, 0);
}

bool open_gl_renderer_cache::texture_has_mip_map(texture_id texture)
{
	if (!m_gl_texture_map_.count(texture))
	{
		HND_CORE_LOG_WARN("RENDERER_CACHE\tTexture does not exist!");
		return false;
	}

	return m_gl_texture_map_[texture].has_mip_map;
}

renderer_cache::frame_buffer_id open_gl_renderer_cache::frame_buffer_create()
{
	const texture_id buffer_color_buffer_texture = texture_create_2d();

	texture_set_2d_filter_mode(buffer_color_buffer_texture, TEXTURE_FILTER_LINEAR);
	
	open_gl_frame_buffer* buffer_instance = object_database::get_instance()->create_object_instance<open_gl_frame_buffer>();
	
	gl_object_id gl_frame_buffer_object = 0;
	glGenFramebuffers(1, &gl_frame_buffer_object);
	m_gl_frame_buffer_map_[buffer_instance->get_object_id()].gl_frame_buffer_object_id = gl_frame_buffer_object;
	m_gl_frame_buffer_map_[buffer_instance->get_object_id()].color_buffer_texture_id = buffer_color_buffer_texture;


	glBindFramebuffer(GL_FRAMEBUFFER, m_gl_frame_buffer_map_[buffer_instance->get_object_id()].gl_frame_buffer_object_id);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_gl_texture_map_[buffer_color_buffer_texture].gl_texture_object_id, 0);
	
	return buffer_instance->get_object_id();
}

void open_gl_renderer_cache::frame_buffer_set_size(frame_buffer_id frame_buffer, const vec2_i& size)
{
	if(!m_gl_frame_buffer_map_.count(frame_buffer))
	{
		HND_CORE_LOG_WARN("RENDERER_CACHE\tFramebuffer not found! Not setting size!");
		return;
	}

	gl_frame_buffer_data& buffer_data = m_gl_frame_buffer_map_[frame_buffer];
	buffer_data.size = size;
	
	glBindFramebuffer(GL_FRAMEBUFFER, buffer_data.gl_frame_buffer_object_id);
		
	glBindTexture(GL_TEXTURE_2D, buffer_data.color_buffer_texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, buffer_data.size.get_x(), buffer_data.size.get_y(), 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

	glBindTexture(GL_TEXTURE_2D, 0);
}

const vec2_i& open_gl_renderer_cache::frame_buffer_get_size(frame_buffer_id frame_buffer)
{
	if (!m_gl_frame_buffer_map_.count(frame_buffer))
	{
		HND_CORE_LOG_WARN("RENDERER_CACHE\tFramebuffer not found! Return size 0");
		return vec2_i::zero();
	}
	
	return m_gl_frame_buffer_map_[frame_buffer].size;
}

gl_object_id open_gl_renderer_cache::get_frame_buffer_object_id(frame_buffer_id frame_buffer)
{
	if (!m_gl_frame_buffer_map_.count(frame_buffer))
	{
		HND_CORE_LOG_WARN("RENDERER_CACHE\tFramebuffer not found! Return gl_object 0");
		return 0;
	}

	return m_gl_frame_buffer_map_[frame_buffer].gl_frame_buffer_object_id;
}

open_gl_renderer_cache::open_gl_renderer_cache()
{
	s_instance_ = this;

	register_module<texture_cache_module, open_gl_texture_cache_module>();
}

open_gl_renderer_cache::~open_gl_renderer_cache()
{
}

GLint open_gl_renderer_cache::texture_get_gl_text_type(texture_type type)
{
	switch(type)
	{
	case TEXTURE_INVALID: return GL_INVALID_VALUE;
	case TEXTURE_TYPE_1D: return GL_TEXTURE_1D;
	case TEXTURE_TYPE_2D: return GL_TEXTURE_2D;
	case TEXTURE_TYPE_3D: return GL_TEXTURE_3D;
	case TEXTURE_TYPE_1D_ARRAY: return GL_TEXTURE_1D_ARRAY;
	case TEXTURE_TYPE_2D_ARRAY: return GL_TEXTURE_2D_ARRAY;
	case TEXTURE_TYPE_CUBE: return GL_TEXTURE_CUBE_MAP;
	case TEXTURE_TYPE_CUBE_ARRAY: return GL_TEXTURE_CUBE_MAP_ARRAY;
	}
}

GLint open_gl_renderer_cache::texture_get_gl_wrap_mode(texture_wrap_mode mode)
{
	switch (mode)
	{
	case TEXTURE_WRAP_CLAMP_TO_EDGE:
		return GL_CLAMP_TO_EDGE;

	case TEXTURE_WRAP_CLAMP_TO_BORDER:
		return GL_CLAMP_TO_BORDER;
	case TEXTURE_WRAP_REPEAT:
		return GL_REPEAT;
	case TEXTURE_WRAP_MIRRORED_REPEAT:
		return GL_MIRRORED_REPEAT;
	case TEXTURE_WRAP_MIRROR_CLAMP_TO_EDGE:
		return GL_MIRROR_CLAMP_TO_EDGE;
	}
}

GLint open_gl_renderer_cache::texture_get_gl_filter_mode(texture_filter_mode mode)
{
	switch (mode) {
	case TEXTURE_FILTER_LINEAR:
		return GL_LINEAR;
	case TEXTURE_FILTER_NEAREST:
		return GL_NEAREST;
	}
}
