#include "hound/hnd_pch.h"
#include "glfw_window.h"

#include <GLFW/glfw3.h>
#include "hound/core/event/window_event.h"
#include "hound/drivers/display_driver.h"
#include "hound/platform/glfw/glfw_display_driver.h"

void glfw_window::init(window_id id, const char* title, GLFWwindow* native_window_handle)
{
	m_window_id_ = id;
	m_native_handle_ = native_window_handle;
	init_window_values(title, native_window_handle);
}

void glfw_window::init_window_values(const char* title, GLFWwindow* native_handle_handle)
{
	m_title_ = std::string(title);
	
	int pos_x, pos_y, width, height;
	glfwGetWindowPos(native_handle_handle, &pos_x, &pos_y);
	glfwGetWindowSize(native_handle_handle, &width, &height);
	m_rect_ = { pos_x, pos_y, width, height };

	int fb_width, fb_height;
	glfwGetFramebufferSize(native_handle_handle, &fb_width, &fb_height);
	m_frame_buffer_rect_ = { 0, 0, fb_height, fb_width };

	float scale_x, scale_y;
	glfwGetWindowContentScale(native_handle_handle, &scale_x, &scale_y);
	m_content_scale_ = { scale_x, scale_y };

	m_min_size_ = vec2_i::zero();
	m_max_size_ = vec2_i::zero();
	glfwSetWindowSizeLimits(native_handle_handle, GLFW_DONT_CARE, GLFW_DONT_CARE, GLFW_DONT_CARE, GLFW_DONT_CARE);
	
	m_aspect_ = vec2_i::zero();
	glfwSetWindowAspectRatio(native_handle_handle, GLFW_DONT_CARE, GLFW_DONT_CARE);
}

void* glfw_window::get_native_handle()
{
	return m_native_handle_;
}

void glfw_window::on_glfw_close_callback()
{
	if (!check_valid_window())
	{
		log_event_error("close");
		return;
	}

	m_should_close_ = true;
	
	window_close_event e{};
	e.set_window_id(m_window_id_);
	e.set_window_object(this);
	e.set_is_main_window(m_window_id_ == display_driver::MAIN_WINDOW_ID);
	e.set_should_close(m_should_close_);
	
	publish_event(e);
}

void glfw_window::on_glfw_size_callback(const vec2_i& size)
{
	if (!check_valid_window())
	{
		log_event_error("resize");
		return;
	}

	m_rect_.set_size(size);
	
	window_resize_event e{};
	e.set_window_id(m_window_id_);
	e.set_window_object(this);
	e.set_rect(m_rect_);

	publish_event(e);
}

void glfw_window::on_glfw_frame_buffer_callback(const vec2_i& size)
{
	if (!check_valid_window())
	{
		log_event_error("framebuffer resize");
		return;
	}

	m_frame_buffer_rect_.set_size(size);
	
	window_frame_buffer_resize_event e{};
	e.set_window_id(m_window_id_);
	e.set_window_object(this);
	e.set_rect(m_frame_buffer_rect_);

	publish_event(e);
}

void glfw_window::on_glfw_content_scale_callback(const vec2_f& scale)
{
	if (!check_valid_window())
	{
		log_event_error("content scale");
		return;
	}

	m_content_scale_ = scale;

	window_content_scale_change_event e{};
	e.set_window_id(m_window_id_);
	e.set_window_object(this);
	e.set_scale(m_content_scale_);

	publish_event(e);
}

void glfw_window::on_glfw_position_callback(const vec2_i& position)
{
	if (!check_valid_window())
	{
		log_event_error("position");
		return;
	}

	m_rect_.set_origin(position);

	if (!display_driver::get_instance()->is_window_on_monitor(m_window_id_, m_monitor_id_))
	{
		const monitor_id new_monitor = display_driver::get_instance()->get_native_monitor(m_window_id_);

		glfw_display_driver* driver = dynamic_cast<glfw_display_driver*>(display_driver::get_instance());
		
		driver->remove_window_from_monitor(m_window_id_, m_monitor_id_);
		driver->add_window_to_monitor(m_window_id_, new_monitor);
		
		m_monitor_id_ = new_monitor;
	}
	
	window_move_event e{};
	e.set_window_id(m_window_id_);
	e.set_window_object(this);
	e.set_rect(m_rect_);

	publish_event(e);
}

void glfw_window::on_glfw_iconified_callback(bool iconified)
{
	if (!check_valid_window())
	{
		log_event_error("minimize");
		return;
	}

	m_is_minimized_ = iconified;

	if (m_is_minimized_)
	{
		switch_mode(window_mode::minimized);
	}
	else
	{
		restore_mode();
	}
	
	window_mode_change_event e{};
	e.set_window_id(m_window_id_);
	e.set_window_object(this);
	e.set_mode(m_mode_);

	publish_event(e);
}

void glfw_window::on_glfw_maximized_callback(bool maximized)
{
	if (!check_valid_window())
	{
		log_event_error("maximize");
		return;
	}

	m_is_maximized_ = maximized;

	if (m_is_minimized_)
	{
		switch_mode(window_mode::maximized);
	}
	else
	{
		restore_mode();
	}
	
	window_mode_change_event e{};
	
	e.set_window_id(m_window_id_);
	e.set_window_object(this);
	e.set_mode(m_mode_);

	publish_event(e);
}

void glfw_window::on_glfw_focused_callback(bool focused)
{
	if (!check_valid_window())
	{
		log_event_error("focus");
		return;
	}

	m_is_focused_ = focused;

	window_focused_event e{};
	e.set_window_id(m_window_id_);
	e.set_window_object(this);
	e.set_is_focused(m_is_focused_);

	publish_event(e);
}

void glfw_window::on_glfw_key_callback(key_code key, key_action action, int mods)
{
	if (!check_valid_window())
	{
		log_event_error("key input");
		return;
	}

	window_key_input_event e{};
	e.set_window_id(m_window_id_);
	e.set_window_object(this);
	e.set_button(key);
	e.set_action(action);
	e.set_mods(mods);

	publish_event(e);
}

void glfw_window::on_glfw_mouse_position_callback(double pos_x, double pos_y)
{
	if (!check_valid_window())
	{
		log_event_error("mouse position");
		return;
	}

	window_mouse_move_event e{};
	e.set_window_id(m_window_id_);
	e.set_window_object(this);
	e.set_mouse_pos({pos_x, pos_y});

	publish_event(e);
}

void glfw_window::on_glfw_mouse_input_callback(key_code key, key_action action, int mods)
{
	if (!check_valid_window())
	{
		log_event_error("mouse input");
		return;
	}

	window_mouse_button_input_event e{};
	e.set_window_id(m_window_id_);
	e.set_window_object(this);
	e.set_button(key);
	e.set_action(action);
	e.set_mods(mods);

	publish_event(e);
}

void glfw_window::begin_frame()
{
	glfwMakeContextCurrent(m_native_handle_);
}

void glfw_window::end_frame()
{
}
