#pragma once
#include "hound/drivers/display_driver.h"

struct GLFWwindow;
struct GLFWmonitor;

class glfw_display_driver : public display_driver
{
	friend class glfw_callback_handler;
	friend class glfw_window;
public:
	glfw_display_driver();
	~glfw_display_driver();
	
protected:
	struct glfw_window_data : window_data
	{
		GLFWwindow* native_window_handle;
	};
	
	void init() override;
	window_id create_window(const char* title, const vec2_i& size, window_id parent_window_id, monitor_id monitor_id) override;
	void destroy_window(window_id window) override;
	window* get_window_handle(window_id window) override;
	void* get_native_window_handle(window_id window) override;
	void* get_native_proc_address() override;
	void redraw_window(window_id window) override;
	void process_window_events() override;

	struct glfw_monitor_data : monitor_data
	{
		GLFWmonitor* native_monitor_handle_;
	};
	
private:
	
	window_id m_window_counter_ = MAIN_WINDOW_ID;
	std::unordered_map<window_id, glfw_window_data> m_window_data_map_;

	glfw_window_data& get_window_data(window_id window);
	glfw_window_data& create_new_window_data();
	GLFWwindow* create_native_window(const char* title, const vec2_i& size, window_id parent_window_id);
	void destroy_native_window(window_id window);

	void native_request_attention(window_id window) override;
	void set_native_title(window_id window, const char* title) override;
	void set_native_rect(window_id window, const rect_i& rect) override;
	void set_native_min_size(window_id window, const vec2_i& size) override;
	void set_native_max_size(window_id window, const vec2_i& size) override;
	void set_native_aspect(window_id window, const vec2_i& aspect) override;
	void set_native_mode(window_id window, window_mode aspect) override;
	void set_native_should_close(window_id window, bool should_close) override;
	void set_native_resizable(window_id window, bool is_resizable) override;
	void set_native_visible(window_id window, bool is_visible) override;
	void set_native_focus(window_id window, bool is_focused) override;
	void set_native_maximized(window_id window, bool is_maximized) override;
	void set_native_minimized(window_id window, bool is_minimized) override;
	void set_native_always_on_top(window_id window, bool is_always_on_top) override;
	void set_native_border_style(window_id window, window_border_style style) override;

	void set_window_active_context(window_id window);

	monitor_id m_monitor_counter_ = MAIN_MONITOR_ID;
	std::unordered_map<monitor_id, glfw_monitor_data> m_monitor_data_map_;
	rect_i m_virtual_screen_size_;

	void identify_monitors();
	monitor* get_monitor_handle(monitor_id monitor) override;
	bool is_window_on_monitor(window_id window, monitor_id monitor) override;
	monitor_id get_native_monitor(window_id window_id) override;
	void add_window_to_monitor(window_id window, monitor_id monitor) override;
	void remove_window_from_monitor(window_id window, monitor_id monitor) override;
};

