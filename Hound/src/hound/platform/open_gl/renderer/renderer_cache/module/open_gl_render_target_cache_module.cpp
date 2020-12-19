#include "hound/hnd_pch.h"
#include "open_gl_render_target_cache_module.h"

#include "hound/platform/open_gl/renderer/renderer_cache/open_gl_renderer_cache.h"
#include "hound/core/rendering/target/viewport.h"
#include "hound/core/rendering/target/render_texture.h"

HND_RENDER_CACHE_BASE_FUNC_IMPL(render_target_cache_module)
HND_RENDER_CACHE_CREATE_FUNC_IMPL_1(render_target_cache_module, viewport, const vec2_i&)
HND_RENDER_CACHE_CREATE_FUNC_IMPL_1(render_target_cache_module, render_texture, const vec2_i&)

void open_gl_render_target_cache_module::on_create_instance(viewport* instance, const vec2_i& size)
{
	gl_viewport_data& data = m_viewport_data_map_[instance->get_object_id()];
	data.id = instance->get_object_id();
	data.handle = instance;
	m_unique_targets_.insert(instance);
}

void open_gl_render_target_cache_module::set_viewport_size(resource_id id, const vec2_i& size)
{
	if(m_viewport_data_map_.count(id))
	{
		HND_CORE_LOG_INFO("Setting viewport framebuffer size!");
	}
}

void open_gl_render_target_cache_module::on_create_instance(render_texture* instance, const vec2_i& size)
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

open_gl_render_target_cache_module::open_gl_render_target_cache_module()
{
}

open_gl_render_target_cache_module::~open_gl_render_target_cache_module()
{
}
