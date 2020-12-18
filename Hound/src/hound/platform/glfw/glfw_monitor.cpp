#include "hound/hnd_pch.h"
#include "glfw_monitor.h"
#include <GLFW/glfw3.h>

void glfw_monitor::init(monitor_id id, struct GLFWmonitor* native_monitor_handle)
{
	m_monitor_id_ = id;
	m_native_handle_ = native_monitor_handle;
	init_values();
}

void glfw_monitor::init_values()
{
	
	int mode_count;
	const GLFWvidmode* video_modes = glfwGetVideoModes(m_native_handle_, &mode_count);

	for (int vi = 0; vi < mode_count; ++vi)
	{
		const GLFWvidmode mode = video_modes[vi];

		add_video_mode(mode);
	}

	int width_mm, height_mm;
	glfwGetMonitorPhysicalSize(m_native_handle_, &width_mm, &height_mm);
	m_physical_size_ = { width_mm, height_mm };

	int work_pos_x, work_pos_y, work_width, work_height;
	glfwGetMonitorWorkarea(m_native_handle_, &work_pos_x, &work_pos_y, &work_width, &work_height);
	m_work_area_ = { work_pos_x, work_pos_y, work_width, work_height };

	const char* name = glfwGetMonitorName(m_native_handle_);
	m_name_ = std::string(name);	
}

void glfw_monitor::add_video_mode(const GLFWvidmode& mode)
{
	m_video_modes_.insert(
		video_mode(
			{ mode.width, mode.height },
			mode.refreshRate,
			mode.redBits,
			mode.greenBits,
			mode.blueBits));
}
