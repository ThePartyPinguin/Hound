#include "hound/hnd_pch.h"
#include "open_gl_render_target_cache_module.h"

#include "hound/platform/open_gl/renderer/renderer_cache/open_gl_renderer_cache.h"
#include "hound/platform/open_gl/renderer/render_target/open_gl_viewport.h"

open_gl_render_target_cache_module::open_gl_render_target_cache_module()
{
}

open_gl_render_target_cache_module::~open_gl_render_target_cache_module()
{
}

render_target* open_gl_render_target_cache_module::get_render_target(render_target_id render_target)
{
	if(!m_render_target_map_.count(render_target))
	{
		HND_CORE_LOG_WARN("Render target does not exist, returning nullptr");
		return nullptr;
	}

	return m_render_target_map_[render_target].target_object;
}

void open_gl_render_target_cache_module::render_target_set_frame_buffer_size(render_target_id render_target, vec2_i size)
{
	if(!m_render_target_map_.count(render_target))
	{
		HND_CORE_LOG_WARN("Could not find render_target, not setting frame buffer size");
		return;
	}

	const auto data = get_render_target_data(render_target);
	renderer_cache::frame_buffer_cache()->frame_buffer_set_size(data.frame_buffer, size);
}

const open_gl_render_target_cache_module::open_gl_render_target_data& open_gl_render_target_cache_module::get_render_target_data(render_target_id render_target)
{
	return m_render_target_map_[render_target];
}

viewport* open_gl_render_target_cache_module::get_viewport(render_target_id render_target)
{
	if(m_render_target_map_.count(render_target))
	{
		HND_CORE_LOG_WARN("Could not find render_target, returning nullptr");
		return nullptr;
	}

	const open_gl_render_target_data& data = get_render_target_data(render_target);

	if(data.type != VIEWPORT)
	{
		HND_CORE_LOG_WARN("render_target is not a viewport, returning nullptr");
		return nullptr;
	}

	return dynamic_cast<viewport*>(data.target_object);
}

render_target_id open_gl_render_target_cache_module::create_render_target(target_type type, const vec2_i& size)
{
	render_target_id id;
	switch (type)
	{
		case RTC::VIEWPORT:
		{
			id = create_view_port(size)->get_object_id();
			break;
		}
		case RTC::TEXTURE:
		{
			HND_CORE_LOG_ERROR("RenderTarget 'TEXTURE' not supported!");
			id = render_target_id::null();
			break;
		}
	}

	m_unique_targets_.insert(id);
	return id;
}

render_target* open_gl_render_target_cache_module::create_view_port(const vec2_i& size)
{
	auto* vp = object_database::get_instance()->create_object_instance<open_gl_viewport>();

	const render_target_id id = vp->get_object_id();

	open_gl_render_target_data& data = m_render_target_map_[id];

	data.id = id;
	data.target_object = vp;
	data.type = VIEWPORT;
	data.frame_buffer = open_gl_renderer_cache::gl_frame_buffer_cache()->frame_buffer_create(size);
	
	return vp;
}

const vec2_i& open_gl_render_target_cache_module::get_render_target_size(render_target_id render_target)
{
	if (!m_render_target_map_.count(render_target))
	{
		HND_CORE_LOG_WARN("Render target does not exist, returning zero size");
		return {0,0};
	}

	return renderer_cache::frame_buffer_cache()->frame_buffer_get_size(m_render_target_map_[render_target].frame_buffer);
}

frame_buffer_id open_gl_render_target_cache_module::get_target_frame_buffer(render_target_id target)
{
	if (!m_render_target_map_.count(target))
	{
		HND_CORE_LOG_WARN("Render target does not exist, returning zero");
		return frame_buffer_id::null();
	}

	return m_render_target_map_[target].frame_buffer;
}

std::set<render_target_id> open_gl_render_target_cache_module::get_render_targets()
{
	return m_unique_targets_;
}
