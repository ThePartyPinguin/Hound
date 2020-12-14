#include "hound/hnd_pch.h"
#include "display_manager_windows.h"

#include "hound/core/object/object_database.h"
#include "hound/core/rendering/render_target/window/window.h"
#include "hound/platform/windows/window_glfw_helper.h"

bool windows_display_manager::s_glfw_initialized = false;

windows_display_manager::windows_display_manager()
{
	m_window_count_ = 0;

	init_glfw();
	identify_monitors();
}

windows_display_manager::~windows_display_manager()
{
	glfwTerminate();
}

void windows_display_manager::on_init()
{
	init_glfw();
	identify_monitors();
}

void windows_display_manager::redraw_windows()
{
	for (auto& pair : m_id_native_window_map_)
	{
		const window_data& data = get_window_data(pair.first);
		if(data.is_visible)
		{
			glfwMakeContextCurrent(pair.second);
			glfwSwapBuffers(pair.second);
		}
	}
}

void windows_display_manager::init_glfw()
{
	if(!glfwInit())
	{
		HND_CORE_LOG_ERROR("ERROR::DISPLAY_MANAGER - Could not initialize glfw!!");
		return;
	}
	glfwSetErrorCallback(glfw_error_callback);

	identify_monitors();
}

void windows_display_manager::identify_monitors()
{
	int monitor_count = 0;
	GLFWmonitor** monitors = glfwGetMonitors(&monitor_count);

	m_monitors_.reserve(monitor_count);

	for (int i = 0; i < monitor_count; ++i)
	{
		GLFWmonitor* monitor = monitors[i];

		monitor_data &md = m_monitors_[m_monitor_counter_];

		md.native_monitor = monitor;
		md.video_modes = glfwGetVideoModes(monitor, &md.mode_count);
		md.id = m_monitor_counter_;

		int pos_x, pos_y;
		glfwGetMonitorPos(monitor, &pos_x, &pos_y);
		md.position = vec2_i(pos_x, pos_y);

		int width, height;
		glfwGetMonitorWorkarea(monitor, &pos_x, &pos_y, &width, &height);
		md.work_area = rect_i(pos_x, pos_y, width, height);

		glfwGetMonitorPhysicalSize(monitor, &width, &height);
		md.physical_size = vec2_i(width, height);

		float scale_x, scale_y;
		glfwGetMonitorContentScale(monitor, &scale_x, &scale_y);
		md.content_scale = vec2_f(scale_x, scale_y);

		md.name = std::string(glfwGetMonitorName(monitor));
		
		++m_monitor_counter_;
	}
}

void windows_display_manager::create_native_window(const window_data& window_data)
{
	GLFWwindow* native_window = create_glfw_window(window_data);
	m_id_native_window_map_[window_data.id] = native_window;
	m_native_window_id_map_[native_window] = window_data.id;
}

GLFWwindow* windows_display_manager::create_glfw_window(const window_data& window_data)
{
	glfwDefaultWindowHints();

	GLFWwindow* window = nullptr;
	GLFWwindow* parent_window = nullptr;

	if (window_data.parent_id != INVALID_WINDOW_ID)
	{
		parent_window = m_id_native_window_map_[window_data.parent_id];
	}
		
	monitor_data& md = m_monitors_[window_data.monitor_id];
	const GLFWvidmode* current_vid_mode = glfwGetVideoMode(md.native_monitor);

	glfwWindowHint(GLFW_RESIZABLE, window_data.is_resizable ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_FLOATING, window_data.is_always_on_top ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_DECORATED, (window_data.border_style == bordered) ? GLFW_TRUE : GLFW_FALSE);

	switch (window_data.mode)
	{
	case windowed:
		window = glfwCreateWindow(window_data.rect.get_width(), window_data.rect.get_height(), window_data.title.c_str(), nullptr, parent_window);
		glfwSetWindowPos(window, window_data.rect.get_x(), window_data.rect.get_y());
		glfwSetWindowSize(window, window_data.rect.get_width(), window_data.rect.get_height());
		break;
	case minimized:
		window = glfwCreateWindow(window_data.rect.get_width(), window_data.rect.get_height(), window_data.title.c_str(), nullptr, parent_window);
		glfwIconifyWindow(window);
		break;
	case maximized:
		window = glfwCreateWindow(window_data.rect.get_width(), window_data.rect.get_height(), window_data.title.c_str(), nullptr, parent_window);
		glfwMaximizeWindow(window);
		break;
	case full_screen:
		window = glfwCreateWindow(current_vid_mode->width, current_vid_mode->height, window_data.title.c_str(), md.native_monitor, parent_window);
		break;
	case full_screen_windowed:
		glfwWindowHint(GLFW_RED_BITS, current_vid_mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, current_vid_mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, current_vid_mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, current_vid_mode->refreshRate);
		glfwWindowHint(GLFW_FLOATING, true);
		glfwWindowHint(GLFW_DECORATED, false);
		window = glfwCreateWindow(current_vid_mode->width, current_vid_mode->height, window_data.title.c_str(), nullptr, parent_window);
		break;
	}
	
	glfwSetWindowCloseCallback(window, glfw_callback_close);
	glfwSetWindowSizeCallback(window, glfw_callback_size);
	glfwSetFramebufferSizeCallback(window, glfw_callback_frame_buffer_size);
	glfwSetWindowContentScaleCallback(window, glfw_callback_content_scale);
	glfwSetWindowPosCallback(window, glfw_callback_position);
	glfwSetWindowIconifyCallback(window, glfw_callback_iconified);
	glfwSetWindowMaximizeCallback(window, glfw_callback_maximized);
	glfwSetWindowFocusCallback(window, glfw_callback_focus);
	glfwSetKeyCallback(window, glfw_callback_key_input);
	glfwSetCharCallback(window, glfw_callback_char_input);
	glfwSetInputMode(window, GLFW_LOCK_KEY_MODS, GLFW_TRUE);
	glfwMakeContextCurrent(window);

	return window;
}

void windows_display_manager::destroy_glfw_window(window_id id)
{
	glfwDestroyWindow(m_id_native_window_map_[id]);

	m_native_window_id_map_.erase(m_id_native_window_map_[id]);
	m_id_native_window_map_.erase(id);
}

void windows_display_manager::redraw_all_native_windows()
{
	for (auto& pair : m_id_native_window_map_)
	{
		glfwMakeContextCurrent(pair.second);
		glfwSwapBuffers(pair.second);
	}
}

void windows_display_manager::redraw_native_window(window_id id)
{
	glfwMakeContextCurrent(m_id_native_window_map_[id]);
	glfwSwapBuffers(m_id_native_window_map_[id]);
}

void windows_display_manager::poll_native_window_events()
{
	glfwPollEvents();
}

void windows_display_manager::set_native_window_title(window_id window, const std::string& title)
{
	GLFWwindow* native_window = m_id_native_window_map_[window];
	glfwSetWindowTitle(native_window, title.c_str());
}

void windows_display_manager::set_native_window_rect(window_id window, const rect_i& rect)
{
	GLFWwindow* native_window = m_id_native_window_map_[window];
	glfwSetWindowPos(native_window, rect.get_x(), rect.get_y());
	glfwSetWindowSize(native_window, rect.get_width(), rect.get_height());
}

void windows_display_manager::set_native_window_frame_buffer_rect(window_id window, const rect_i& rect)
{
	
}

void windows_display_manager::set_native_window_min_size(window_id window, const vec2_i& min_size)
{
	const window_data& md = get_window_data(window);
	GLFWwindow* native_window = m_id_native_window_map_[window];
	glfwSetWindowSizeLimits(native_window, min_size.get_x(), min_size.get_y(), md.max_size.get_x(), md.max_size.get_y());
}

void windows_display_manager::set_native_window_max_size(window_id window, const vec2_i& max_size)
{
	const window_data& md = get_window_data(window);
	GLFWwindow* native_window = m_id_native_window_map_[window];
	glfwSetWindowSizeLimits(native_window, md.max_size.get_x(), md.max_size.get_y(), max_size.get_x(), max_size.get_y());
}

void windows_display_manager::set_native_window_mode(window_id window, window_mode mode)
{
	GLFWwindow* native_window = m_id_native_window_map_[window];
	const window_data& wd = get_window_data(window);

	monitor_data& md = m_monitors_[wd.monitor_id];

	const GLFWvidmode* vid_mode = glfwGetVideoMode(md.native_monitor);


	switch (mode)
	{
	case windowed:
	{
		glfwSetWindowMonitor(native_window, nullptr, wd.rect.get_x(), wd.rect.get_y(), wd.rect.get_width(), wd.rect.get_height(), 0);
		break;
	}
	case minimized:
	{
		glfwIconifyWindow(native_window);
		break;
	}
	case maximized:
	{
		glfwMaximizeWindow(native_window);
		break;
	}
	case full_screen:
	case full_screen_windowed:
	{

		glfwSetWindowMonitor(native_window, md.native_monitor, 0, 0, vid_mode->width, vid_mode->height, vid_mode->refreshRate);
		break;
	}
	}
}

void windows_display_manager::set_native_window_visible(window_id window, bool visible)
{
	GLFWwindow* native_window = m_id_native_window_map_[window];
	if (visible)
	{
		glfwShowWindow(native_window);
	}
	else
	{
		glfwHideWindow(native_window);
	}
}

void windows_display_manager::set_native_window_focused(window_id window, bool focused)
{
	GLFWwindow* native_window = m_id_native_window_map_[window];
	if (focused)
	{
		glfwFocusWindow(native_window);
	}
}

void windows_display_manager::set_native_window_resizable(window_id window, bool is_resizable)
{
	GLFWwindow* native_window = m_id_native_window_map_[window];
	glfwSetWindowAttrib(native_window, GLFW_RESIZABLE, is_resizable ? GLFW_TRUE : GLFW_FALSE);
}

void windows_display_manager::set_native_window_content_scale(window_id window, const vec2_f& scale)
{
}

void windows_display_manager::set_native_window_aspect(window_id window, const vec2_i& aspect)
{
	GLFWwindow* native_window = m_id_native_window_map_[window];
	glfwSetWindowAspectRatio(native_window, aspect.get_x(), aspect.get_y());
}

void windows_display_manager::set_native_window_maximized(window_id window, bool maximized)
{
	GLFWwindow* native_window = m_id_native_window_map_[window];
	
	if(glfwGetWindowAttrib(native_window, GLFW_MAXIMIZED) != (maximized ? GLFW_TRUE : GLFW_FALSE))
		glfwMaximizeWindow(native_window);
}

void windows_display_manager::set_native_window_minimized(window_id window, bool minimized)
{
	GLFWwindow* native_window = m_id_native_window_map_[window];
	
	if(glfwGetWindowAttrib(native_window, GLFW_ICONIFIED) != (minimized ? GLFW_TRUE : GLFW_FALSE))
		glfwIconifyWindow(native_window);
}

void windows_display_manager::set_native_window_is_always_on_top(window_id window, bool is_always_on_top)
{
	GLFWwindow* native_window = m_id_native_window_map_[window];

	if(glfwGetWindowAttrib(native_window, GLFW_FLOATING) != (is_always_on_top ? GLFW_TRUE : GLFW_FALSE))
		glfwSetWindowAttrib(native_window, GLFW_FLOATING, is_always_on_top ? GLFW_TRUE : GLFW_FALSE);
}

void windows_display_manager::set_native_window_border_style(window_id window, window_border_style style)
{
	GLFWwindow* native_window = m_id_native_window_map_[window];
	if (glfwGetWindowAttrib(native_window, GLFW_DECORATED) != ((style == bordered) ? GLFW_TRUE : GLFW_FALSE))
		glfwSetWindowAttrib(native_window, GLFW_DECORATED, (style == bordered) ? GLFW_TRUE : GLFW_FALSE);
}

void windows_display_manager::native_window_request_attention(window_id window)
{
	GLFWwindow* native_window = m_id_native_window_map_[window];
	glfwRequestWindowAttention(native_window);
}

void windows_display_manager::glfw_callback_close(GLFWwindow* window)
{
	auto* _this = dynamic_cast<windows_display_manager*>(s_instance_);

	const window_id id = _this->m_native_window_id_map_[window];
	const window_data& wd = _this->get_window_data(id);

	if (id == INVALID_WINDOW_ID)
	{
		return;
	}
	
	wd.window_object->set_should_close(true);
}

void windows_display_manager::glfw_callback_size(GLFWwindow* window, int width, int height)
{
	auto* _this = dynamic_cast<windows_display_manager*>(s_instance_);

	const window_id id = _this->m_native_window_id_map_[window];
	const window_data& wd = _this->get_window_data(id);

	if (wd.window_object == nullptr || id == INVALID_WINDOW_ID)
	{
		return;
	}
	
	int pos_x, pos_y;

	glfwGetWindowPos(window, &pos_x, &pos_y);

	wd.window_object->set_rect({ pos_x, pos_y, width, height });
}

void windows_display_manager::glfw_callback_frame_buffer_size(GLFWwindow* window, int width, int height)
{
	auto* _this = dynamic_cast<windows_display_manager*>(s_instance_);

	const window_id id = _this->m_native_window_id_map_[window];
	const window_data& wd = _this->get_window_data(id);

	if (id == INVALID_WINDOW_ID)
	{
		return;
	}
	_this->set_window_frame_buffer(id, { 0, 0, width, height });
}

void windows_display_manager::glfw_callback_content_scale(GLFWwindow* window, float scale_x, float scale_y)
{
	auto* _this = dynamic_cast<windows_display_manager*>(s_instance_);

	const window_id id = _this->m_native_window_id_map_[window];
	const window_data& wd = _this->get_window_data(id);

	if (id == INVALID_WINDOW_ID)
	{
		return;
	}

	_this->set_window_content_scale(id, { scale_x, scale_y });
}

void windows_display_manager::glfw_callback_position(GLFWwindow* window, int pos_x, int pos_y)
{
	auto* _this = dynamic_cast<windows_display_manager*>(s_instance_);

	const window_id id = _this->m_native_window_id_map_[window];
	const window_data& wd = _this->get_window_data(id);

	if (id == INVALID_WINDOW_ID)
	{
		return;
	}
	
	int width, height;

	glfwGetWindowSize(window, &width, &height);

	wd.window_object->set_rect({ pos_x, pos_y, width, height });
}

void windows_display_manager::glfw_callback_iconified(GLFWwindow* window, int iconified)
{
	auto* _this = dynamic_cast<windows_display_manager*>(s_instance_);

	const window_id id = _this->m_native_window_id_map_[window];
	const window_data& wd = _this->get_window_data(id);

	if (id == INVALID_WINDOW_ID)
	{
		return;
	}
	
	wd.window_object->set_is_minimized(iconified);
}

void windows_display_manager::glfw_callback_maximized(GLFWwindow* window, int maximized)
{
	auto* _this = dynamic_cast<windows_display_manager*>(s_instance_);

	const window_id id = _this->m_native_window_id_map_[window];
	const window_data& wd = _this->get_window_data(id);

	if (id == INVALID_WINDOW_ID)
	{
		return;
	}

	wd.window_object->set_is_maximized(maximized);
}

void windows_display_manager::glfw_callback_focus(GLFWwindow* window, int focused)
{
	auto* _this = dynamic_cast<windows_display_manager*>(s_instance_);

	const window_id id = _this->m_native_window_id_map_[window];
	const window_data& wd = _this->get_window_data(id);

	if (id == INVALID_WINDOW_ID)
	{
		return;
	}

	wd.window_object->set_is_focused(focused);
}

void windows_display_manager::glfw_callback_key_input(GLFWwindow* window, int key, int scan_code, int action, int mods)
{
	auto* _this = dynamic_cast<windows_display_manager*>(s_instance_);

	const window_id id = _this->m_native_window_id_map_[window];
	const window_data& wd = _this->get_window_data(id);

	if (id == INVALID_WINDOW_ID)
	{
		return;
	}

	const key_code kc = windows_glfw_helper::get_key_code(key);
	const key_action ka = windows_glfw_helper::get_key_action(action);

	_this->pass_key_event(wd.window_object, id, kc, ka, mods);
}

void windows_display_manager::glfw_callback_mouse_position(GLFWwindow* window, double pos_x, double pos_y)
{
	auto* _this = dynamic_cast<windows_display_manager*>(s_instance_);

	const window_id id = _this->m_native_window_id_map_[window];
	const window_data& wd = _this->get_window_data(id);

	if (id == INVALID_WINDOW_ID)
	{
		return;
	}

	_this->pass_mouse_position_event(wd.window_object, id, pos_x, pos_y);
}

void windows_display_manager::glfw_callback_mouse_button_input(GLFWwindow* window, int button, int action, int mods)
{
	auto* _this = dynamic_cast<windows_display_manager*>(s_instance_);

	const window_id id = _this->m_native_window_id_map_[window];
	const window_data& wd = _this->get_window_data(id);

	if (id == INVALID_WINDOW_ID)
	{
		return;
	}

	const key_code bc = windows_glfw_helper::get_mouse_button(button);
	const key_action ka = windows_glfw_helper::get_key_action(action);

	_this->pass_mouse_button_event(wd.window_object, id, bc, ka, mods);
}

void windows_display_manager::glfw_callback_char_input(GLFWwindow* window, unsigned code_point)
{
}


void windows_display_manager::glfw_error_callback(int error, const char* description)
{
	HND_CORE_LOG_ERROR("ERROR::GLFW\r\n\tError code:", error, "\r\n\tDescription: ", description);
}