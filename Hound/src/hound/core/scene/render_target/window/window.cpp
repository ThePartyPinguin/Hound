#include "hound/hnd_pch.h"
#include "window.h"

#include "hound/core/input/input_event.h"
#include "hound/core/event/window_event.h"
#include "hound/core/object/object_database.h"

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

window::window()
{	
}

window::~window()
{
}

void window::on_input_event(const input_event_with_modifier& event)
{
}

void window::on_window_event(const window_event& event)
{
	HND_CORE_LOG_INFO("Event received in window class");
}

void window::close()
{
	if (m_window_id_ == display_manager::INVALID_WINDOW_ID)
	{
		return;
	}
	window_close_event e{};
	e.set_window_id(m_window_id_);
	e.set_window_object(this);
	e.set_is_main_window(m_window_id_ == display_manager::MAIN_WINDOW_ID);

	publish_event<window_close_event>(e);
}

void window::show()
{
	m_is_visible_ = true;
	if (m_window_id_ == display_manager::get_instance()->INVALID_WINDOW_ID)
	{
		return;
	}
	display_manager::get_instance()->show_window(m_window_id_);
	
	window_visibility_event e{};
	e.set_window_id(m_window_id_);
	e.set_window_object(this);
	e.set_is_visible(m_is_visible_);
	publish_event<window_visibility_event>(e);
}

void window::hide()
{
	m_is_visible_ = false;
	if (m_window_id_ == display_manager::get_instance()->INVALID_WINDOW_ID)
	{
		return;
	}
	display_manager::get_instance()->hide_window(m_window_id_);

	window_visibility_event e{};
	e.set_window_id(m_window_id_);
	e.set_window_object(this);
	e.set_is_visible(m_is_visible_);
	publish_event<window_visibility_event>(e);
}

void window::grab_focus()
{
	if (m_window_id_ == display_manager::get_instance()->INVALID_WINDOW_ID)
	{
		return;
	}
	display_manager::get_instance()->grab_focus(m_window_id_);
}

void window::on_set_title(const std::string& title)
{
	if(m_window_id_ == display_manager::get_instance()->INVALID_WINDOW_ID)
	{
		return;
	}
	display_manager::get_instance()->set_window_title(m_window_id_, title);

	window_title_event e{};
	e.set_window_id(m_window_id_);
	e.set_window_object(this);
	e.set_title(title);
	publish_event<window_title_event>(e);
}

void window::on_set_rect(const rect_i& rect)
{
	if (m_window_id_ == display_manager::get_instance()->INVALID_WINDOW_ID)
	{
		return;
	}
	display_manager::get_instance()->set_window_rect(m_window_id_, rect);

	window_resize_event e{};
	e.set_window_id(m_window_id_);
	e.set_window_object(this);
	e.set_type(window_event::RESIZE);
	e.set_rect(m_rect_);
	
	publish_event<window_resize_event>(e);
}

void window::on_set_frame_buffer_rect(const rect_i& rect)
{
	if (m_window_id_ == display_manager::get_instance()->INVALID_WINDOW_ID)
	{
		return;
	}
	window_frame_buffer_resize_event e{};
	e.set_window_id(m_window_id_);
	e.set_window_object(this);
	e.set_type(window_event::RESIZE);
	e.set_size(m_rect_.get_size());

	publish_event<window_frame_buffer_resize_event>(e);
}

void window::on_set_content_scale(const vec2_f& scale)
{
	if (m_window_id_ == display_manager::get_instance()->INVALID_WINDOW_ID)
	{
		return;
	}
	window_content_scale_update_event e{};
	e.set_window_id(m_window_id_);
	e.set_window_object(this);
	e.set_type(window_event::RESIZE);
	e.set_scale(scale);

	publish_event<window_content_scale_update_event>(e);
}

void window::on_set_min_size(const vec2_i& size)
{
	if (m_window_id_ == display_manager::get_instance()->INVALID_WINDOW_ID)
	{
		return;
	}
	display_manager::get_instance()->set_window_min_size(m_window_id_, size);
}

void window::on_set_max_size(const vec2_i& size)
{
	if (m_window_id_ == display_manager::get_instance()->INVALID_WINDOW_ID)
	{
		return;
	}
	display_manager::get_instance()->set_window_max_size(m_window_id_, size);
}

void window::on_set_mode(const mode& mode)
{
	if (m_window_id_ == display_manager::get_instance()->INVALID_WINDOW_ID)
	{
		return;
	}
	display_manager::get_instance()->set_window_mode(m_window_id_, static_cast<display_manager::window_mode>(mode));

	window_mode_change_event e{};
	e.set_window_id(m_window_id_);
	e.set_window_object(this);
	e.set_type(window_event::RESIZE);
	e.set_mode(static_cast<int>(mode));

	publish_event<window_mode_change_event>(e);
}

void window::on_set_flags(const int& flags)
{
	m_window_flags_ = flags;
	if (m_window_id_ == display_manager::get_instance()->INVALID_WINDOW_ID)
	{
		return;
	}
	display_manager::get_instance()->set_window_flags(m_window_id_, flags);
}

void window::on_set_visible(const bool& visible)
{
	m_is_visible_ = visible;
	if (m_window_id_ == display_manager::get_instance()->INVALID_WINDOW_ID)
	{
		return;
	}
	
	display_manager::get_instance()->set_window_visible(m_window_id_, visible);

	window_visibility_event e{};
	e.set_window_id(m_window_id_);
	e.set_window_object(this);
	e.set_is_visible(visible);

	publish_event(e);
}

void window::on_set_focus(const bool& focus)
{
	m_is_focused_ = focus;
	if (m_window_id_ == display_manager::get_instance()->INVALID_WINDOW_ID)
	{
		return;
	}
	display_manager::get_instance()->set_window_focus(m_window_id_, focus);

	window_focused_event e{};
	e.set_window_id(m_window_id_);
	e.set_window_object(this);
	e.set_is_focused(focus);

	publish_event(e);
}
