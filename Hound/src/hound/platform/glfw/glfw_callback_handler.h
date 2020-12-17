#pragma once

struct GLFWwindow;

class glfw_callback_handler
{
	friend class glfw_display_driver;
	
	static void glfw_callback_close(GLFWwindow* window);
	static void glfw_callback_size(GLFWwindow* window, int width, int height);
	static void glfw_callback_frame_buffer_size(GLFWwindow* window, int width, int height);
	static void glfw_callback_content_scale(GLFWwindow* window, float scale_x, float scale_y);
	static void glfw_callback_position(GLFWwindow* window, int pos_x, int pos_y);
	static void glfw_callback_iconified(GLFWwindow* window, int iconified);
	static void glfw_callback_maximized(GLFWwindow* window, int maximized);
	static void glfw_callback_focus(GLFWwindow* window, int focused);

	static void glfw_callback_key_input(GLFWwindow* window, int key, int scan_code, int action, int mods);
	static void glfw_callback_mouse_position(GLFWwindow* window, double pos_x, double pos_y);
	static void glfw_callback_mouse_button_input(GLFWwindow* window, int button, int action, int mods);
	static void glfw_callback_char_input(GLFWwindow* window, unsigned int code_point);
};

