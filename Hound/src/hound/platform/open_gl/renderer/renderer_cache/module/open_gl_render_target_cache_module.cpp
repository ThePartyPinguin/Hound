#include "hound/hnd_pch.h"
#include "open_gl_render_target_cache_module.h"

#include "hound/platform/open_gl/renderer/renderer_cache/open_gl_renderer_cache.h"
#include "hound/core/rendering/target/viewport.h"
#include "hound/core/rendering/target/render_texture.h"

RENDER_CACHE_CLASS_IMPL(render_target_cache_module, render_target)
RENDER_CACHE_CREATE_FUNC_IMPL_P1(viewport, render_target_cache_module, const rect_i&, rect)
RENDER_CACHE_CREATE_FUNC_IMPL_P1(render_texture, render_target_cache_module, const rect_i&, size)


void open_gl_render_target_cache_module::viewport_begin_frame(resource_id id)
{
	const rect_i& vp_rect = m_viewport_data_map_[id].vp_rect;
	glViewport(vp_rect.get_x(), vp_rect.get_y(), vp_rect.get_width(), vp_rect.get_height());
}

void open_gl_render_target_cache_module::on_create_instance(viewport* instance, const rect_i& rect)
{
	gl_viewport_data& data = m_viewport_data_map_[instance->get_object_id()];
	data.id = instance->get_object_id();
	data.handle = instance;
	data.vp_rect = rect;
	m_unique_targets_.insert(instance);
}

void open_gl_render_target_cache_module::set_viewport_rect(resource_id id, const rect_i& rect)
{
	if(m_viewport_data_map_.count(id))
	{
		m_viewport_data_map_[id].vp_rect = rect;
	}
}

void open_gl_render_target_cache_module::on_create_instance(render_texture* instance, const rect_i& rect)
{
	gl_render_texture_data& data = m_render_texture_data_map_[instance->get_object_id()];
	data.id = instance->get_object_id();
	data.handle = instance;
	m_unique_targets_.insert(instance);
}

std::set<render_target*> open_gl_render_target_cache_module::get_render_targets()
{
	return m_unique_targets_;
}

void open_gl_render_target_cache_module::init()
{
}

open_gl_render_target_cache_module::open_gl_render_target_cache_module()
{
	s_instance_ = this;
}

open_gl_render_target_cache_module::~open_gl_render_target_cache_module()
{
}
