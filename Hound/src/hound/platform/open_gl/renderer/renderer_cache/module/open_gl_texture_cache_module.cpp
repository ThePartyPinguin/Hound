#include "hound/hnd_pch.h"
#include "open_gl_texture_cache_module.h"

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

texture_cache_module::texture_id open_gl_texture_cache_module::texture_create_2d()
{
	return object_id::null();
}

void open_gl_texture_cache_module::texture_set_2d_size(texture_id texture, const vec2_i& size)
{
}

void open_gl_texture_cache_module::texture_set_2d_filter_mode(texture_id texture, texture_filter_mode mode)
{
}

void open_gl_texture_cache_module::texture_set_2d_wrap_mode(texture_id texture, texture_wrap_mode h,texture_wrap_mode v)
{
}

void open_gl_texture_cache_module::texture_set_2d_data(texture_id texture, unsigned char* data)
{
}

unsigned char* open_gl_texture_cache_module::get_texture_2d_data(texture_id texture, vec2_i& size)
{
	return nullptr;
}

void open_gl_texture_cache_module::texture_generate_2d_mip_map(texture_id texture)
{
}

bool open_gl_texture_cache_module::texture_has_mip_map(texture_id texture)
{
	return false;
}
