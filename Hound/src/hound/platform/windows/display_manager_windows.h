#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "hound/display/display_manager.h"

class windows_display_manager : public display_manager
{
public:	
	windows_display_manager();
	virtual ~windows_display_manager();
	
private:
	monitor_id m_monitor_counter_ = MAIN_MONITOR_ID;

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

	std::unordered_map<monitor_id, monitor_data> m_monitor_data_map_;

	uint32_t m_window_count_ = MAIN_WINDOW_ID;

	struct glfw_window_data : window_data
	{
		GLFWwindow* native_window_handle;
	};

	std::unordered_map<window_id, glfw_window_data> m_window_data_map_;
	
	static bool s_glfw_initialized;

	void init_glfw();
	void identify_monitors();

	glfw_window_data& create_window(window_id id);
	
	const window_data& get_main_window() override;
	const window_data& get_window_data(window_id window) override;
	
	void redraw_windows() override;
	void redraw_window(window_id id) override;
	const window_data& request_sub_window(const std::string& title, const vec2_i& size, window_id parent_id) override;
	bool destroy_sub_window(window_id id) override;

	void process_window_events() override;
	
	void window_set_title(window_id window, const std::string& title) override;
	void window_set_rect(window_id window, const rect_i& rect) override;
	void window_set_frame_buffer_rect(window_id window, const rect_i& rect) override;
	void window_set_min_size(window_id window, const vec2_i& min_size) override;
	void window_set_max_size(window_id window, const vec2_i& max_size) override;
	void window_set_mode(window_id window, window_mode mode) override;
	void window_set_visible(window_id window, bool visible) override;
	void window_set_focused(window_id window, bool focused) override;
	void window_set_resizable(window_id window, bool is_resizable) override;
	void window_set_content_scale(window_id window, const vec2_f& scale) override;
	void window_set_aspect(window_id window, const vec2_i& aspect) override;
	void window_set_maximized(window_id window, bool maximized) override;
	void window_set_minimized(window_id window, bool minimized) override;
	void window_set_is_always_on_top(window_id window, bool is_always_on_top) override;
	void window_set_border_style(window_id window, window_border_style style) override;
	void window_request_attention(window_id window) override;

	
	rect_i monitor_centered_rect(const vec2_i& size, monitor_id monitor = MAIN_MONITOR_ID);
	
	//GLFW IMPLEMENTATION

	std::unordered_map<GLFWwindow*, window_id> m_native_window_id_map_;
	
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
