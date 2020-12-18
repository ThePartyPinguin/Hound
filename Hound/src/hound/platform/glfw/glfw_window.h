#pragma once
#include "hound/core/window/window.h"
#include "hound/core/input/key_codes.h"

struct GLFWwindow;
struct GLFWmonitor;

class glfw_window : public window
{
	friend class glfw_callback_handler;
	friend class glfw_display_driver;

	GLFWwindow* m_native_handle_;
	GLFWmonitor* m_native_monitor_;
	
	void init(window_id id, const char* title, GLFWwindow* native_window_handle);
	void init_window_values(const char* title, GLFWwindow* native_handle_handle);
	void* get_native_handle() override;

	void set_monitor();
	
	void on_glfw_close_callback();
	void on_glfw_size_callback(const vec2_i& size);
	void on_glfw_frame_buffer_callback(const vec2_i& size);
	void on_glfw_content_scale_callback(const vec2_f& scale);
	void on_glfw_position_callback(const vec2_i& position);
	void on_glfw_iconified_callback(bool iconified);
	void on_glfw_maximized_callback(bool maximized);
	void on_glfw_focused_callback(bool focused);
	void on_glfw_key_callback(key_code key, key_action action, int mods);
	void on_glfw_mouse_position_callback(double pos_x, double pos_y);
	void on_glfw_mouse_input_callback(key_code key, key_action action, int mods);
};

