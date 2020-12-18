#pragma once
#include "hound/core/window/monitor.h"

struct GLFWvidmode;
struct GLFWmonitor;

class glfw_monitor : public monitor
{
	friend class glfw_display_driver;

private:
	GLFWmonitor* m_native_handle_;

	void init(monitor_id id, GLFWmonitor* native_monitor_handle);
	void init_values();
	void add_video_mode(const GLFWvidmode& mode);
	
public:
	glfw_monitor() = default;
	~glfw_monitor() = default;
};

