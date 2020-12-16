#include "hound/hnd_pch.h"
#include "window.h"

#include "hound/core/input/input_event.h"
#include "hound/core/event/window_event.h"
#include "hound/core/object/object_database.h"

window* window::create(const std::string& title, vec2_i size, window_id parent_id)
{
	const display_manager::window_data& data = display_manager::get_instance()->request_sub_window(title, size, parent_id);
	return data.window_object;
}

void window::show()
{
	set_is_visible(true);
}

void window::hide()
{
	set_is_visible(false);
}

void window::maximize()
{
	set_is_maximized(true);
}

void window::minimize()
{
	set_is_minimized(true);
}

void window::restore()
{
	set_is_maximized(false);
	set_is_minimized(false);
	set_is_visible(true);
}

void window::grab_focus()
{
	set_is_focused(true);
}

void window::resize(const rect_i& rect)
{
	set_rect(rect);
}

void window::request_attention()
{
	display_manager::get_instance()->window_request_attention(m_window_id_);
}

void window::close()
{
	set_should_close(true);
}

window::window()
{	
}

window::~window()
{
}

void window::dpm_set_frame_buffer_rect(const rect_i& rect)
{
	if (m_window_id_ == display_manager::get_instance()->INVALID_WINDOW_ID)
	{
		HND_CORE_LOG_WARN("Window has an invalid window_id");
		return;
	}

	window_frame_buffer_resize_event e{};
	e.set_window_id(m_window_id_);
	e.set_window_object(this);
	e.set_rect(rect);

	publish_event(e);
}

void window::dpm_set_aspect(const vec2_i& aspect)
{
	if (m_window_id_ == display_manager::get_instance()->INVALID_WINDOW_ID)
	{
		HND_CORE_LOG_WARN("Window has an invalid window_id");
		return;
	}
	
	on_set_aspect(aspect);
}

void window::dpm_set_content_scale(const vec2_f& scale)
{
	if (m_window_id_ == display_manager::get_instance()->INVALID_WINDOW_ID)
	{
		HND_CORE_LOG_WARN("Window has an invalid window_id");
		return;
	}
	
	window_content_scale_change_event e{};
	e.set_window_id(m_window_id_);
	e.set_window_object(this);
	e.set_scale(scale);

	publish_event(e);
}

void window::on_set_title(const std::string& title)
{
	if (m_window_id_ == display_manager::get_instance()->INVALID_WINDOW_ID)
	{
		HND_CORE_LOG_WARN("Window has an invalid window_id");
		return;
	}

	display_manager::get_instance()->window_set_title(m_window_id_, title);
	
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
		HND_CORE_LOG_WARN("Window has an invalid window_id");
		return;
	}

	display_manager::get_instance()->window_set_rect(m_window_id_, rect);

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
		HND_CORE_LOG_WARN("Window has an invalid window_id");
		return;
	}
	
	display_manager::get_instance()->window_set_min_size(m_window_id_, size);

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
		HND_CORE_LOG_WARN("Window has an invalid window_id");
		return;
	}

	display_manager::get_instance()->window_set_max_size(m_window_id_, size);

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
		HND_CORE_LOG_WARN("Window has an invalid window_id");
		return;
	}

	display_manager::get_instance()->window_set_aspect(m_window_id_, aspect);

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
		HND_CORE_LOG_WARN("Window has an invalid window_id");
		return;
	}

	display_manager::get_instance()->window_set_mode(m_window_id_, static_cast<display_manager::window_mode>(mode));

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
		HND_CORE_LOG_WARN("Window has an invalid window_id");
		return;
	}

	//TODO Set should close tag
	
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
		HND_CORE_LOG_WARN("Window has an invalid window_id");
		return;
	}

	display_manager::get_instance()->window_set_resizable(m_window_id_, resizable);
	
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
		HND_CORE_LOG_WARN("Window has an invalid window_id");
		return;
	}

	display_manager::get_instance()->window_set_visible(m_window_id_, visible);

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
		HND_CORE_LOG_WARN("Window has an invalid window_id");
		return;
	}
	
	display_manager::get_instance()->window_set_focused(m_window_id_, focus);

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
		HND_CORE_LOG_WARN("Window has an invalid window_id");
		return;
	}
	
	display_manager::get_instance()->window_set_maximized(m_window_id_, is_maximized);

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
		HND_CORE_LOG_WARN("Window has an invalid window_id");
		return;
	}

	display_manager::get_instance()->window_set_minimized(m_window_id_, is_minimized);

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
		HND_CORE_LOG_WARN("Window has an invalid window_id");
		return;
	}
	
	display_manager::get_instance()->window_set_is_always_on_top(m_window_id_, is_always_on_top);
	
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
		HND_CORE_LOG_WARN("Window has an invalid window_id");
		return;
	}

	display_manager::get_instance()->window_set_border_style(m_window_id_, static_cast<display_manager::window_border_style>(style));
	
	window_border_style_change_event e{};
	e.set_window_id(m_window_id_);
	e.set_window_object(this);
	e.set_border_style(static_cast<int>(style));

	publish_event(e);
}
