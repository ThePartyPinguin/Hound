#pragma once
#include "hound/managers/display_manager.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

class windows_display_manager : public display_manager
{
public:	
	windows_display_manager();
	virtual ~windows_display_manager();
	
private:
	uint32_t m_window_count_;

	void on_init() override;
	
	void init_glfw();
	void identify_monitors();
	
	struct monitor_data
	{
		GLFWmonitor* native_monitor;
		const GLFWvidmode* video_modes;
		int mode_count;
		
		monitor_id id;
		vec2_i position;
		rect_i work_area;
		vec2_i physical_size;
		vec2_f content_scale;
		std::string name;
	};
	
	static bool s_glfw_initialized;

	void create_native_window(const window_data& window_data) override;

	void redraw_all_native_windows() override;
	void redraw_native_window(window_id id) override;

	void poll_native_window_events() override;
	
	void set_native_window_title(window_id window, const std::string& title) override;
	void set_native_window_rect(window_id window, const rect_i& rect) override;
	void set_native_window_min_size(window_id window, const vec2_i& min_size) override;
	void set_native_window_max_size(window_id window, const vec2_i& max_size) override;
	void set_native_window_mode(window_id window, window_mode mode) override;
	void set_native_window_visible(window_id window, bool visible) override;
	void set_native_window_focused(window_id window, bool focused) override;
	void set_native_window_maximized(window_id window, bool maximized) override;
	void set_native_window_minimized(window_id window, bool minimized) override;
	void set_native_window_is_always_on_top(window_id window, bool is_always_on_top) override;
	void set_native_window_border_style(window_id window, window_border_style style) override;

	std::unordered_map<monitor_id, monitor_data> m_monitors_;
	monitor_id m_monitor_counter_ = MAIN_MONITOR_ID;

	std::unordered_map<window_id, GLFWwindow*> m_id_native_window_map_;

	std::unordered_map<GLFWwindow*, window_id> m_native_window_id_map_;
	
	//GLFW IMPLEMENTATION
	GLFWwindow* create_glfw_window(const window_data& window_data);
	void destroy_glfw_window(window_id id);
	
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

	static void glfw_error_callback(int error, const char* description);
};
