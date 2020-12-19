#include "hound/hnd_pch.h"
#include "open_gl_frame_buffer_cache_module.h"

#include "hound/platform/open_gl/renderer/renderer_cache/open_gl_renderer_cache.h"
#include "hound/platform/open_gl/renderer/render_target/open_gl_frame_buffer.h"

frame_buffer_id HND_GL_FBC::frame_buffer_create(const vec2_i& size)
{
	auto* texture_module = open_gl_renderer_cache::gl_texture_cache();
	
	const texture_id buffer_color_buffer_texture = texture_module->texture_create_2d(size);

	texture_module->texture_set_2d_filter_mode(buffer_color_buffer_texture, texture_cache_module::TEXTURE_FILTER_LINEAR);
	
	auto* buffer_instance = object_database::get_instance()->create_object_instance<open_gl_frame_buffer>();

	const frame_buffer_id id = buffer_instance->get_object_id();
	
	gl_frame_buffer_data& data = m_gl_frame_buffer_map_[id];
	
	data.color_buffer_texture_id = buffer_color_buffer_texture;

	HND_GL_CALL(glGenFramebuffers, 1, &data.gl_frame_buffer_object_id);
	HND_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, data.gl_frame_buffer_object_id);

	const HND_GL_TCM::gl_texture_data& texture_data = texture_module->get_gl_texture_data(buffer_color_buffer_texture);

	//Bind created texture as the colorBuffer
	HND_GL_CALL(glFramebufferTexture2D, GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_data.gl_texture_object_id, 0);

	//Create depthBuffer
	HND_GL_CALL(glGenRenderbuffers, 1, &data.gl_depth_stencil_object_id);
	HND_GL_CALL(glBindRenderbuffer, GL_RENDERBUFFER, data.gl_depth_stencil_object_id);
	HND_GL_CALL(glRenderbufferStorage, GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.get_x(), size.get_y());
	HND_GL_CALL(glBindRenderbuffer, GL_RENDERBUFFER, 0);

	HND_GL_CALL(glFramebufferRenderbuffer, GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, data.gl_depth_stencil_object_id);
	
	const GLint frame_buffer_status = HND_GL_CALL(glCheckFramebufferStatus, GL_FRAMEBUFFER);
	
	if (frame_buffer_status != GL_FRAMEBUFFER_COMPLETE)
	{
		const char* status_msg = get_gl_frame_buffer_status_msg(frame_buffer_status);
		HND_CORE_LOG_WARN("Framebuffer created but not complete!\tStatus:", status_msg);
	}
	
	return buffer_instance->get_object_id();
}

void HND_GL_FBC::frame_buffer_set_size(frame_buffer_id frame_buffer, const vec2_i& size)
{
	if (!m_gl_frame_buffer_map_.count(frame_buffer))
	{
		HND_CORE_LOG_WARN("RENDERER_CACHE\tFramebuffer not found! Not setting size!");
		return;
	}

	gl_frame_buffer_data& buffer_data = m_gl_frame_buffer_map_[frame_buffer];
	buffer_data.size = size;

	HND_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, buffer_data.gl_frame_buffer_object_id);
	
	auto* texture_module = open_gl_renderer_cache::gl_texture_cache();

	texture_module->texture_set_2d_size(buffer_data.color_buffer_texture_id, size);

	HND_GL_CALL(glBindTexture, GL_TEXTURE_2D, 0);

	if (HND_GL_CALL(glCheckFramebufferStatus, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		HND_CORE_LOG_WARN("Framebuffer size set but not complete!");
	}
}

const vec2_i& HND_GL_FBC::frame_buffer_get_size(frame_buffer_id frame_buffer)
{
	if (!m_gl_frame_buffer_map_.count(frame_buffer))
	{
		HND_CORE_LOG_WARN("RENDERER_CACHE\tFramebuffer not found! Return size 0");
		return vec2_i::zero();
	}

	return m_gl_frame_buffer_map_[frame_buffer].size;
}

const HND_GL_FBC::gl_frame_buffer_data& HND_GL_FBC::get_gl_frame_buffer_data(frame_buffer_id frame_buffer)
{
	return m_gl_frame_buffer_map_[frame_buffer];
}

void open_gl_frame_buffer_cache_module::bind_gl_frame_buffer(frame_buffer_id frame_buffer)
{
	HND_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, get_gl_frame_buffer_data(frame_buffer).gl_frame_buffer_object_id);
}

void open_gl_frame_buffer_cache_module::un_bind_gl_frame_buffer(frame_buffer_id frame_buffer)
{
	HND_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, 0);
}

mesh_id open_gl_frame_buffer_cache_module::get_frame_buffer_quad()
{
	return m_frame_buffer_mesh_id_;
}

HND_GL_FBC::open_gl_frame_buffer_cache_module()
{
	m_frame_buffer_mesh_id_ = open_gl_renderer_cache::gl_mesh_cache()->mesh_create();

	const mesh_cache_module::mesh_data data
	{
		{
			{ {1.0f,  1.0f, 0.0f}, {}, {1.0f, 1.0f}},
			{ {1.0f, -1.0f, 0.0f}, {}, {1.0f, 0.0f}},
			{ {-1.0f, -1.0f, 0.0f}, {}, {0.0f, 0.0f}},
			{ {-1.0f, 1.0f, 0.0f}, {}, {0.0f, 1.0f}},
		},
		{
			0, 1, 3,
			1, 2, 3
		}
	};

	
	open_gl_renderer_cache::gl_mesh_cache()->mesh_add_data(m_frame_buffer_mesh_id_, data);
}

HND_GL_FBC::~open_gl_frame_buffer_cache_module()
{
}

const char* open_gl_frame_buffer_cache_module::get_gl_frame_buffer_status_msg(GLint status)
{
	switch (status)
	{
		case GL_FRAMEBUFFER_UNDEFINED:
		{
			return "FRAME_BUFFER_UNDEFINED";
		}
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		{
			return "FRAME_BUFFER_INCOMPLETE_ATTACHMENT";
		}
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		{
			return "FRAME_BUFFER_INCOMPLETE_MISSING_ATTACHMENT";
		}
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		{
			return "FRAME_BUFFER_INCOMPLETE_DRAW_BUFFER";
		}
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		{
			return "FRAME_BUFFER_INCOMPLETE_READ_BUFFER";
		}
		case GL_FRAMEBUFFER_UNSUPPORTED:
		{
			return "FRAME_BUFFER_UNSUPPORTED";
		}
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
		{
			return "FRAME_BUFFER_INCOMPLETE_MULTISAMPLE";
		}
		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
		{
			return "FRAME_BUFFER_INCOMPLETE_LAYER_TARGETS";
		}
		default: return "FRAME_BUFFER_STATUS_UNKNOWN";
	}
}
