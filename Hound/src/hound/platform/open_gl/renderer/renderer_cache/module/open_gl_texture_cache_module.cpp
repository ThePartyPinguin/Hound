#include "hound/hnd_pch.h"
#include "open_gl_texture_cache_module.h"

#include "hound/core/object/object_database.h"
#include "hound/core/object/texture/texture.h"

open_gl_texture_cache_module::open_gl_texture_cache_module()
{
}

open_gl_texture_cache_module::~open_gl_texture_cache_module()
{
	for(auto& pair : m_gl_texture_map_)
	{
		glDeleteTextures(1, &pair.second.gl_texture_object_id);
	}
}

texture_id open_gl_texture_cache_module::texture_create_2d(const vec2_i& size)
{
	texture* texture_instance = object_database::get_instance()->create_object_instance<texture>();

	gl_object_id gl_texture_object;
	glGenTextures(1, &gl_texture_object);

	texture_id id = texture_instance->get_object_id();
	
	gl_texture_data& texture = m_gl_texture_map_[id];
	texture.gl_texture_object_id = gl_texture_object;
	texture.type = TEXTURE_TYPE_2D;

	texture_set_2d_size(id, size);

	return texture_instance->get_object_id();
}

void open_gl_texture_cache_module::texture_set_2d_size(texture_id texture, const vec2_i& size)
{
	if (!m_gl_texture_map_.count(texture))
	{
		HND_CORE_LOG_WARN("RENDERER_CACHE\tTexture does not exist!");
		return;
	}

	gl_texture_data& data = m_gl_texture_map_[texture];

	if (data.type != TEXTURE_TYPE_2D)
	{
		HND_CORE_LOG_WARN("RENDERER_CACHE\tTexture is not a 2d texture, could not set size!");
		return;
	}

	data.size = size;

	glBindTexture(GL_TEXTURE_2D, data.gl_texture_object_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, data.size.get_x(), data.size.get_y(), 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
}

void open_gl_texture_cache_module::texture_set_2d_filter_mode(texture_id texture, texture_filter_mode mode)
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

void open_gl_texture_cache_module::texture_set_2d_wrap_mode(texture_id texture, texture_wrap_mode h,texture_wrap_mode v)
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

void open_gl_texture_cache_module::texture_set_2d_data(texture_id texture, unsigned char* tex_data_buffer)
{
	gl_texture_data& texture_data = m_gl_texture_map_[texture];

	if (texture_data.type != TEXTURE_TYPE_2D)
	{
		HND_CORE_LOG_WARN("RENDERER_CACHE\tTexture is not a 2d texture, could not set size!");
		return;
	}

	glBindTexture(GL_TEXTURE_2D, texture_data.gl_texture_object_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_data.size.get_x(), texture_data.size.get_y(), 0, GL_RGB, GL_UNSIGNED_BYTE, tex_data_buffer);

	glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned char* open_gl_texture_cache_module::get_texture_2d_data(texture_id texture, vec2_i& size)
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

void open_gl_texture_cache_module::texture_generate_2d_mip_map(texture_id texture)
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

bool open_gl_texture_cache_module::texture_has_mip_map(texture_id texture)
{
	if (!m_gl_texture_map_.count(texture))
	{
		HND_CORE_LOG_WARN("RENDERER_CACHE\tTexture does not exist!");
		return false;
	}

	return m_gl_texture_map_[texture].has_mip_map;
}

void open_gl_texture_cache_module::bind_texture(texture_id texture)
{
	if(!m_gl_texture_map_.count(texture))
	{
		HND_CORE_LOG_WARN("Texture does not exists, no texture bound!");
		return;
	}

	const gl_texture_data& data = get_gl_texture_data(texture);

	GLint gl_texture_type;
	
	switch (data.type)
	{
	case TEXTURE_TYPE_2D: gl_texture_type = GL_TEXTURE_2D; break;
	}

	glBindTexture(gl_texture_type, data.gl_texture_object_id);
}

void open_gl_texture_cache_module::un_bind_texture(texture_id texture)
{
	if (!m_gl_texture_map_.count(texture))
	{
		HND_CORE_LOG_WARN("Texture does not exists, no texture bound!");
		return;
	}

	const gl_texture_data& data = get_gl_texture_data(texture);

	GLint gl_texture_type;

	switch (data.type)
	{
	case TEXTURE_TYPE_2D: gl_texture_type = GL_TEXTURE_2D; break;
	}

	glBindTexture(gl_texture_type, data.gl_texture_object_id);
}

const open_gl_texture_cache_module::gl_texture_data& open_gl_texture_cache_module::get_gl_texture_data(texture_id texture)
{
	return m_gl_texture_map_[texture];
}

GLint open_gl_texture_cache_module::texture_get_gl_text_type(texture_type type)
{
	switch (type)
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

GLint open_gl_texture_cache_module::texture_get_gl_wrap_mode(texture_wrap_mode mode)
{
	switch (mode)
	{
		case TEXTURE_WRAP_CLAMP_TO_EDGE: return GL_CLAMP_TO_EDGE;
		case TEXTURE_WRAP_CLAMP_TO_BORDER: return GL_CLAMP_TO_BORDER;
		case TEXTURE_WRAP_REPEAT: return GL_REPEAT;
		case TEXTURE_WRAP_MIRRORED_REPEAT: return GL_MIRRORED_REPEAT;
		case TEXTURE_WRAP_MIRROR_CLAMP_TO_EDGE: return GL_MIRROR_CLAMP_TO_EDGE;
	}
}

GLint open_gl_texture_cache_module::texture_get_gl_filter_mode(texture_filter_mode mode)
{
	switch (mode)
	{
		case TEXTURE_FILTER_LINEAR: return GL_LINEAR;
		case TEXTURE_FILTER_NEAREST: return GL_NEAREST;
	}
}