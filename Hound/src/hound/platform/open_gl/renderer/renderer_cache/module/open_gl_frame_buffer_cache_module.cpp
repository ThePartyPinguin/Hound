#include "hound/hnd_pch.h"
#include "open_gl_frame_buffer_cache_module.h"

#include "hound/core/rendering/renderer_cache.h"
#include "hound/platform/open_gl/renderer/render_target/open_gl_frame_buffer.h"
#include "hound/platform/open_gl/renderer/renderer_cache/module/open_gl_texture_cache_module.h"

frame_buffer_id GL_FBC::frame_buffer_create(const vec2_i& size)
{
	auto* texture_module = dynamic_cast<GL_TCM*>(renderer_cache::get_module<texture_cache_module>());
	
	const texture_id buffer_color_buffer_texture = texture_module->texture_create_2d(size);

	texture_module->texture_set_2d_filter_mode(buffer_color_buffer_texture, texture_cache_module::TEXTURE_FILTER_LINEAR);
	
	auto* buffer_instance = object_database::get_instance()->create_object_instance<open_gl_frame_buffer>();

	gl_object_id gl_frame_buffer_object = 0;
	glGenFramebuffers(1, &gl_frame_buffer_object);

	const frame_buffer_id id = buffer_instance->get_object_id();
	
	gl_frame_buffer_data& data = m_gl_frame_buffer_map_[id];
	
	data.gl_frame_buffer_object_id = gl_frame_buffer_object;
	data.color_buffer_texture_id = buffer_color_buffer_texture;

	glBindFramebuffer(GL_FRAMEBUFFER, data.gl_frame_buffer_object_id);

	const GL_TCM::gl_texture_data& texture_data = texture_module->get_gl_texture_data(buffer_color_buffer_texture);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_data.gl_texture_object_id, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		HND_CORE_LOG_WARN("Framebuffer created but not complete!");
	}
	else
	{
		HND_CORE_LOG_INFO("Frame buffer successfully created");
	}
	
	return buffer_instance->get_object_id();
}

void GL_FBC::frame_buffer_set_size(frame_buffer_id frame_buffer, const vec2_i& size)
{
	if (!m_gl_frame_buffer_map_.count(frame_buffer))
	{
		HND_CORE_LOG_WARN("RENDERER_CACHE\tFramebuffer not found! Not setting size!");
		return;
	}

	gl_frame_buffer_data& buffer_data = m_gl_frame_buffer_map_[frame_buffer];
	buffer_data.size = size;

	glBindFramebuffer(GL_FRAMEBUFFER, buffer_data.gl_frame_buffer_object_id);
	
	auto* texture_module = dynamic_cast<GL_TCM*>(renderer_cache::get_module<texture_cache_module>());

	texture_module->texture_set_2d_size(buffer_data.color_buffer_texture_id, size);

	glBindTexture(GL_TEXTURE_2D, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		HND_CORE_LOG_WARN("Framebuffer size set but not complete!");
	}
}

const vec2_i& GL_FBC::frame_buffer_get_size(frame_buffer_id frame_buffer)
{
	if (!m_gl_frame_buffer_map_.count(frame_buffer))
	{
		HND_CORE_LOG_WARN("RENDERER_CACHE\tFramebuffer not found! Return size 0");
		return vec2_i::zero();
	}

	return m_gl_frame_buffer_map_[frame_buffer].size;
}

const GL_FBC::gl_frame_buffer_data& GL_FBC::get_gl_frame_buffer_data(frame_buffer_id frame_buffer)
{
	return m_gl_frame_buffer_map_[frame_buffer];
}

GL_FBC::open_gl_frame_buffer_cache_module()
{
	
}

GL_FBC::~open_gl_frame_buffer_cache_module()
{
}
