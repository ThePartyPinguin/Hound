#include "hound/hnd_pch.h"
#include "viewport.h"

#include "hound/core/window/window.h"
#include "hound/core/rendering/renderer_cache/module/render_target_cache_module.h"
#include "hound/core/rendering/camera/camera.h"

GET_CACHE_FUNC_IMPL(viewport, render_target_cache_module)
CACHED_OBJECT_CREATE_FUNC_IMPL_P1(viewport, render_target_cache_module, const rect_i&, rect)

void viewport::init(const rect_i& rect)
{
	m_rect_ = rect;
	m_frame_buffer_ = frame_buffer::create(m_rect_.get_size());
}


void viewport::set_owner_window(window* window)
{
	m_owner_window_ = window;
	m_rect_ = m_owner_window_->get_rect();
	
	m_frame_buffer_->set_size(m_rect_.get_size());

	if(m_publisher_ != nullptr)
		m_publisher_->un_subscribe(this);

	window->subscribe<window_frame_buffer_resize_event>(this);
}

void viewport::begin_frame()
{
	get_cache()->viewport_begin_frame(get_object_id());
	m_owner_window_->begin_frame();
	m_frame_buffer_->bind();
	if (m_camera_ != nullptr)
		m_camera_->begin_frame();
}

void viewport::end_frame()
{
	m_owner_window_->end_frame();
	m_frame_buffer_->un_bind();
	if (m_camera_ != nullptr)
		m_camera_->begin_frame();
}

bool viewport::should_render()
{
	return m_owner_window_ != nullptr && m_owner_window_->get_is_visible() && !m_owner_window_->get_is_minimized();
}

void viewport::on_event(const window_frame_buffer_resize_event& e)
{
	const vec2_i& size = e.get_rect().get_size();
	m_frame_buffer_->set_size(size);
	
	set_rect(e.get_rect());
	
	if (m_camera_ != nullptr)
		m_camera_->set_render_target_size(vec2_f(size.get_x(), size.get_y()));
}

void viewport::on_set_rect(const rect_i& rect)
{
	get_cache()->set_viewport_rect(get_object_id(), rect);
}

viewport::viewport()
{
	m_target_type_ = VIEWPORT;
	m_frame_buffer_ = nullptr;
}

