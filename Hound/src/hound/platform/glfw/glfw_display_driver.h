#pragma once
#include "hound/drivers/display_driver.h"

struct GLFWwindow;

class glfw_display_driver : public display_driver
{
	friend class glfw_callback_handler;
public:
	glfw_display_driver();
	~glfw_display_driver();
	
protected:
	struct glfw_window_data : window_data
	{
		GLFWwindow* native_window_handle;
	};
	
	void init() override;
	window_id create_window(const char* title, const vec2_i& size, window_id parent_window_id) override;
	void destroy_window(window_id window) override;
	window* get_window_handle(window_id window) override;
	void* get_native_window_handle(window_id window) override;
	void* get_native_proc_address() override;
	void redraw_window(window_id window) override;
	void process_window_events() override;

private:
	window_id m_window_counter_ = MAIN_WINDOW_ID;
	std::unordered_map<window_id, glfw_window_data> m_window_data_map_;

	glfw_window_data& get_window_data(window_id window);
	glfw_window_data& create_new_window_data();
	GLFWwindow* create_native_window(const char* title, const vec2_i& size, window_id parent_window_id);
	void destroy_native_window(window_id window);
};

