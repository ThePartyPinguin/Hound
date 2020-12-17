#include "hound/hnd_pch.h"
#include "window.h"

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
	display_driver::get_instance()->native_request_attention(m_window_id_);
}

void window::close()
{
	set_should_close(true);
}

void window::on_set_title(const std::string& title) const
{
	if (!check_valid_window())
	{
		log_set_error("title");
		return;
	}
	display_driver::get_instance()->set_native_title(m_window_id_, title.c_str());
}

void window::on_set_rect(const rect_i& rect) const
{
	if (!check_valid_window())
	{
		log_set_error("rect");
		return;
	}
	display_driver::get_instance()->set_native_rect(m_window_id_, rect);
}

void window::on_set_min_size(const vec2_i& size) const
{
	if (!check_valid_window())
	{
		log_set_error("min size");
		return;
	}
	display_driver::get_instance()->set_native_min_size(m_window_id_, size);
}

void window::on_set_max_size(const vec2_i& size) const
{
	if (!check_valid_window())
	{
		log_set_error("max size");
		return;
	}
	display_driver::get_instance()->set_native_max_size(m_window_id_, size);
}

void window::on_set_aspect(const vec2_i& aspect) const
{
	if (!check_valid_window())
	{
		log_set_error("aspect");
		return;
	}
	display_driver::get_instance()->set_native_aspect(m_window_id_, aspect);
}

void window::on_set_mode(const window_mode& mode) const
{
	if (!check_valid_window())
	{
		log_set_error("window mode");
		return;
	}
	display_driver::get_instance()->set_native_mode(m_window_id_, mode);
}

void window::on_set_should_close(bool close) const
{
	if (!check_valid_window())
	{
		log_set_error("close flag");
		return;
	}
	display_driver::get_instance()->set_native_should_close(m_window_id_, close);
}

void window::on_set_resizable(bool resizable) const
{
	if (!check_valid_window())
	{
		log_set_error("resizable flag");
		return;
	}
	display_driver::get_instance()->set_native_resizable(m_window_id_, resizable);
}

void window::on_set_visible(const bool& visible) const
{
	if (!check_valid_window())
	{
		log_set_error("visible flag");
		return;
	}
	display_driver::get_instance()->set_native_visible(m_window_id_, visible);
}

void window::on_set_focus(const bool& focus) const
{
	if (!check_valid_window())
	{
		log_set_error("focus flag");
		return;
	}
	display_driver::get_instance()->set_native_focus(m_window_id_, focus);
}

void window::on_set_maximized(const bool& is_maximized) const
{
	if (!check_valid_window())
	{
		log_set_error("maximized flag");
		return;
	}
	display_driver::get_instance()->set_native_maximized(m_window_id_, is_maximized);
}

void window::on_set_minimized(const bool& is_minimized) const
{
	if (!check_valid_window())
	{
		log_set_error("minimized flag");
		return;
	}
	
	display_driver::get_instance()->set_native_minimized(m_window_id_, is_minimized);
}

void window::on_set_always_on_top(const bool& is_always_on_top) const
{
	if (!check_valid_window())
	{
		log_set_error("always on top flag");
		return;
	}
	
	display_driver::get_instance()->set_native_always_on_top(m_window_id_, is_always_on_top);
}

void window::on_set_border_style(const window_border_style& style) const
{
	if (!check_valid_window())
	{
		log_set_error("border style");
		return;
	}

	display_driver::get_instance()->set_native_border_style(m_window_id_, style);
}

bool window::check_valid_window() const
{
	return m_window_id_ != display_driver::INVALID_WINDOW_ID;
}

void window::log_event_error(const char* error) const
{
	HND_CORE_LOG_WARN("Window is not valid! Could not publish '", error, "' event");
}

void window::log_set_error(const char* error) const
{
	HND_CORE_LOG_WARN("Window is not valid! Not setting native '", error, '\'');
}
