#include "hound/hnd_pch.h"
#include "viewport.h"

#include "hound/core/window/window.h"
#include "hound/core/rendering/renderer_cache/module/render_target_cache_module.h"

GET_CACHE_FUNC_IMPL(viewport, render_target_cache_module)
CACHED_OBJECT_CREATE_FUNC_IMPL_P1(viewport, render_target_cache_module, const vec2_i&, size)

void viewport::set_owner_window(window* window)
{
	m_owner_window_ = window;
	m_size_ = m_owner_window_->get_rect().get_size();
	if(m_frame_buffer_ == nullptr)
	{
		m_frame_buffer_ = frame_buffer::create(m_size_);
	}
	else
	{
		m_frame_buffer_->set_size(m_size_);
	}
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
	m_frame_buffer_ = nullptr;
}

void viewport::on_set_size(const vec2_i& size)
{
	if(m_frame_buffer_ != nullptr)
		m_frame_buffer_->set_size(size);
	
	get_cache()->set_viewport_size(get_object_id(), size);
}
