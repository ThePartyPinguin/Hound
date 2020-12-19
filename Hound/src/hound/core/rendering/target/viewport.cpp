#include "hound/hnd_pch.h"
#include "viewport.h"

#include "hound/core/window/window.h"
#include "hound/core/rendering/renderer_cache/module/render_target_cache_module.h"

HND_OBJECT_CLASS_FUNC_IMPL_1(viewport, render_target_cache_module, const vec2_i&)

void viewport::set_owner_window(window* window)
{
	m_owner_window_ = window;
	set_size(m_owner_window_->get_rect().get_size());
}

void viewport::begin_frame()
{
	m_owner_window_->begin_frame();
	m_frame_buffer_->bind();
}

void viewport::end_frame()
{
	m_owner_window_->end_frame();
	m_frame_buffer_->un_bind();
}

viewport::viewport()
{
	m_target_type_ = VIEWPORT;
	m_frame_buffer_ = frame_buffer::create(m_size_);
}

void viewport::on_set_size(const vec2_i& size)
{
	m_frame_buffer_->set_size(size);
	get_cache()->set_viewport_size(get_object_id(), size);
}
