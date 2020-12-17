#include "hound/hnd_pch.h"
#include "glfw_callback_handler.h"

#include <GLFW/glfw3.h>

#include "glfw_window.h"
#include "hound/platform/windows/window_glfw_helper.h"

void glfw_callback_handler::glfw_callback_close(GLFWwindow* window)
{
	glfw_window* window_object = get_user_pointer_window(window);
	window_object->on_glfw_close_callback();
}

void glfw_callback_handler::glfw_callback_size(GLFWwindow* window, int width, int height)
{
	glfw_window* window_object = get_user_pointer_window(window);
	window_object->on_glfw_size_callback({width, height});
}

void glfw_callback_handler::glfw_callback_frame_buffer_size(GLFWwindow* window, int width, int height)
{
	glfw_window* window_object = get_user_pointer_window(window);
	window_object->on_glfw_frame_buffer_callback({ width, height });
}

void glfw_callback_handler::glfw_callback_content_scale(GLFWwindow* window, float scale_x, float scale_y)
{
	glfw_window* window_object = get_user_pointer_window(window);
	window_object->on_glfw_content_scale_callback({ scale_x, scale_y });
}

void glfw_callback_handler::glfw_callback_position(GLFWwindow* window, int pos_x, int pos_y)
{
	glfw_window* window_object = get_user_pointer_window(window);
	window_object->on_glfw_position_callback({ pos_x, pos_y });
}

void glfw_callback_handler::glfw_callback_iconified(GLFWwindow* window, int iconified)
{
	glfw_window* window_object = get_user_pointer_window(window);
	window_object->on_glfw_iconified_callback(iconified);
}

void glfw_callback_handler::glfw_callback_maximized(GLFWwindow* window, int maximized)
{
	glfw_window* window_object = get_user_pointer_window(window);
	window_object->on_glfw_maximized_callback(maximized);
}

void glfw_callback_handler::glfw_callback_focus(GLFWwindow* window, int focused)
{
	glfw_window* window_object = get_user_pointer_window(window);
	window_object->on_glfw_focused_callback(focused);
}

void glfw_callback_handler::glfw_callback_key_input(GLFWwindow* window, int key, int scan_code, int action, int mods)
{
	glfw_window* window_object = get_user_pointer_window(window);
	
	const key_code kc = windows_glfw_helper::get_key_code(key);
	const key_action ka = windows_glfw_helper::get_key_action(action);

	window_object->on_glfw_key_callback(kc, ka, mods);
}

void glfw_callback_handler::glfw_callback_mouse_position(GLFWwindow* window, double pos_x, double pos_y)
{
	glfw_window* window_object = get_user_pointer_window(window);
	window_object->on_glfw_mouse_position_callback(pos_x, pos_y);
}

void glfw_callback_handler::glfw_callback_mouse_button_input(GLFWwindow* window, int button, int action, int mods)
{
	glfw_window* window_object = get_user_pointer_window(window);

	const key_code kc = windows_glfw_helper::get_mouse_button(button);
	const key_action ka = windows_glfw_helper::get_key_action(action);
	
	window_object->on_glfw_mouse_input_callback(kc, ka, mods);
}

void glfw_callback_handler::glfw_callback_char_input(GLFWwindow* window, unsigned code_point)
{
}

glfw_window* glfw_callback_handler::get_user_pointer_window(struct GLFWwindow* native_window)
{
	return static_cast<glfw_window*>(glfwGetWindowUserPointer(native_window));
}
