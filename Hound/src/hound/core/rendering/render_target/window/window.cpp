#include "hound/hnd_pch.h"
#include "window.h"

#include "hound/core/input/input_event.h"
#include "hound/core/event/window_event.h"
#include "hound/core/object/object_database.h"
#include "hound/core/rendering/renderer_cache.h"

window* window::create(window* parent)
{
	display_manager::window_id parent_id = display_manager::MAIN_WINDOW_ID;

	if (parent)
	{
		parent_id = parent->get_window_id();
	}

	display_manager::window_properties properties = display_manager::get_default_properties();
	window* instance = object_database::get_instance()->create_object_instance<window>();
	
	display_manager::window_id window_id = display_manager::get_instance()->request_sub_window(parent_id, properties);

	instance->m_window_id_ = window_id;
	instance->m_parent_ = parent;
	
	return instance;
}

const rect_i& window::get_rect_() const
{
	return display_manager::get_instance()->get_window_data(m_window_id_).rect;
}

window::window()
{	
}

window::~window()
{
}

void window::dpm_set_frame_buffer_rect(const rect_i& rect)
{
	m_frame_buffer_rect_ = rect;
	if (m_window_id_ == display_manager::get_instance()->INVALID_WINDOW_ID)
	{
		return;
	}

	renderer_cache::get_module<frame_buffer_cache_module>()->frame_buffer_set_size(get_frame_buffer(), rect.get_size());
	
	window_frame_buffer_resize_event e{};
	e.set_window_id(m_window_id_);
	e.set_window_object(this);
	e.set_rect(rect);

	publish_event(e);
}

void window::dpm_set_aspect(const vec2_i& aspect)
{
	m_aspect_ = aspect;
	on_set_aspect(aspect);
}

void window::dpm_set_content_scale(const vec2_f& scale)
{
	m_content_scale_ = scale;
	
}

void window::on_set_title(const std::string& title)
{
	if (m_window_id_ == display_manager::get_instance()->INVALID_WINDOW_ID)
	{
		return;
	}
	
	window_title_event e{};
	e.set_window_id(m_window_id_);
	e.set_window_object(this);
	e.set_title(title);

	publish_event(e);
}

void window::on_set_rect(const rect_i& rect)
{
	if (m_window_id_ == display_manager::get_instance()->INVALID_WINDOW_ID)
	{
		return;
	}

	window_resize_event e{};
	e.set_window_id(m_window_id_);
	e.set_window_object(this);
	e.set_rect(rect);

	publish_event(e);
}

void window::on_set_min_size(const vec2_i& size)
{
	if (m_window_id_ == display_manager::get_instance()->INVALID_WINDOW_ID)
	{
		return;
	}

	window_min_size_change_event e{};
	e.set_window_id(m_window_id_);
	e.set_window_object(this);
	e.set_min_size_(size);

	publish_event(e);
}

void window::on_set_max_size(const vec2_i& size)
{
	if (m_window_id_ == display_manager::get_instance()->INVALID_WINDOW_ID)
	{
		return;
	}

	window_max_size_change_event e{};
	e.set_window_id(m_window_id_);
	e.set_window_object(this);
	e.set_max_size_(size);

	publish_event(e);
}

void window::on_set_aspect(const vec2_i& aspect)
{
	if (m_window_id_ == display_manager::get_instance()->INVALID_WINDOW_ID)
	{
		return;
	}

	window_aspect_change_event e{};
	e.set_window_id(m_window_id_);
	e.set_window_object(this);
	e.set_aspect(aspect);

	publish_event(e);
}

void window::on_set_mode(const mode& mode)
{
	if (m_window_id_ == display_manager::get_instance()->INVALID_WINDOW_ID)
	{
		return;
	}

	window_mode_change_event e{};
	e.set_window_id(m_window_id_);
	e.set_window_object(this);
	e.set_mode(static_cast<int>(mode));

	publish_event(e);
}

void window::on_set_should_close(bool close)
{
	if (m_window_id_ == display_manager::INVALID_WINDOW_ID)
	{
		return;
	}

	window_close_event e{};
	e.set_window_id(m_window_id_);
	e.set_window_object(this);
	e.set_is_main_window(m_window_id_ == display_manager::MAIN_WINDOW_ID);
	e.set_should_close(close);
	publish_event(e);
}

void window::on_set_resizable(bool resizable)
{
	if (m_window_id_ == display_manager::get_instance()->INVALID_WINDOW_ID)
	{
		return;
	}

	window_resizable_event e{};
	e.set_window_id(m_window_id_);
	e.set_window_object(this);
	e.set_is_resizable(resizable);

	publish_event(e);
}

void window::on_set_visible(const bool& visible)
{
	if (m_window_id_ == display_manager::get_instance()->INVALID_WINDOW_ID)
	{
		return;
	}
	
	window_visibility_event e{};
	e.set_window_id(m_window_id_);
	e.set_window_object(this);
	e.set_is_visible(visible);

	publish_event(e);
}

void window::on_set_focus(const bool& focus)
{
	if (m_window_id_ == display_manager::get_instance()->INVALID_WINDOW_ID)
	{
		return;
	}

	window_focused_event e{};
	e.set_window_id(m_window_id_);
	e.set_window_object(this);
	e.set_is_focused(focus);

	publish_event(e);
}

void window::on_set_maximized(const bool& is_maximized)
{
	if (m_window_id_ == display_manager::get_instance()->INVALID_WINDOW_ID)
	{
		return;
	}

	window_maximize_event e{};
	e.set_window_id(m_window_id_);
	e.set_window_object(this);
	e.set_is_maximized(is_maximized);

	publish_event(e);
}

void window::on_set_minimized(const bool& is_minimized)
{
	if (m_window_id_ == display_manager::get_instance()->INVALID_WINDOW_ID)
	{
		return;
	}

	window_minimize_event e{};
	e.set_window_id(m_window_id_);
	e.set_window_object(this);
	e.set_is_minimized(is_minimized);

	publish_event(e);
}

void window::on_set_always_on_top(const bool& is_always_on_top)
{
	if (m_window_id_ == display_manager::get_instance()->INVALID_WINDOW_ID)
	{
		return;
	}

	window_always_on_top_change_event e{};
	e.set_window_id(m_window_id_);
	e.set_window_object(this);
	e.set_always_on_top(is_always_on_top);

	publish_event(e);
}

void window::on_set_border_style(const border_style& style)
{
	if (m_window_id_ == display_manager::get_instance()->INVALID_WINDOW_ID)
	{
		return;
	}

	window_border_style_change_event e{};
	e.set_window_id(m_window_id_);
	e.set_window_object(this);
	e.set_border_style(static_cast<int>(style));

	publish_event(e);
}
