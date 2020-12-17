#include "hound/hnd_pch.h"
#include "display_manager_windows.h"

#include "hound/core/window/window.h"
#include "hound/core/object/object_database.h"
#include "hound/core/rendering/renderer_cache.h"
#include "hound/core/rendering/renderer_cache/module/render_target_cache_module.h"
#include "hound/platform/windows/window_glfw_helper.h"

#include "hound/config/engine.h"

bool windows_display_manager::s_glfw_initialized = false;

windows_display_manager::windows_display_manager()
{
	s_instance_ = this;
	m_window_count_ = 0;

	init_glfw();
	identify_monitors();
	
	create_window(MAIN_WINDOW_ID);
	++m_window_count_;
}

windows_display_manager::~windows_display_manager()
{
	glfwTerminate();
}

void windows_display_manager::redraw_windows()
{
	for (auto& pair : m_window_data_map_)
	{
		redraw_window(pair.first);
	}
}

void windows_display_manager::redraw_window(window_id id)
{
	const glfw_window_data& data = m_window_data_map_[id];
	if (data.is_visible)
	{
		glfwMakeContextCurrent(data.native_window_handle);
		glfwSwapBuffers(data.native_window_handle);
	}
}

const display_manager::window_data& windows_display_manager::request_sub_window(const std::string& title, const vec2_i& size, window_id parent_id)
{
	const window_id window_id = m_window_count_;

	glfw_window_data& wd = create_window(window_id);

	++m_window_count_;

	glfw_window_data& parent_data = m_window_data_map_[parent_id];
	parent_data.children.insert(wd.parent_id);
	return wd;
}

bool windows_display_manager::destroy_sub_window(window_id id)
{
	if (id == INVALID_WINDOW_ID)
		return false;
	
	const window_data& data = get_window_data(id);

	if(data.parent_id != INVALID_WINDOW_ID)
	{
		window_data& parent_data = m_window_data_map_[data.parent_id];
		parent_data.children.erase(data.id);
	}

	destroy_glfw_window(id);

	m_window_data_map_.erase(id);

	return true;
}

void windows_display_manager::process_window_events()
{
	glfwPollEvents();
}

void windows_display_manager::init_glfw()
{
	if(!glfwInit())
	{
		HND_CORE_LOG_ERROR("ERROR::DISPLAY_MANAGER - Could not initialize glfw");
		return;
	}
	glfwSetErrorCallback(glfw_error_callback);
}

void windows_display_manager::identify_monitors()
{
	int monitor_count = 0;
	GLFWmonitor** monitors = glfwGetMonitors(&monitor_count);

	m_monitor_data_map_.reserve(monitor_count);

	for (int i = 0; i < monitor_count; ++i)
	{
		GLFWmonitor* monitor = monitors[i];

		monitor_data &md = m_monitor_data_map_[m_monitor_counter_];

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

windows_display_manager::glfw_window_data& windows_display_manager::create_window(window_id id)
{
	glfw_window_data& wd = m_window_data_map_[id];
	wd.id = id;
	wd.rect = monitor_centered_rect({ 1280, 720 });
	wd.parent_id = INVALID_WINDOW_ID;
	wd.mode = windowed;
	wd.is_visible = true;
	wd.is_focused = true;
	wd.border_style = bordered;
	wd.is_resizable = true;
	
	GLFWwindow* native_widow = create_glfw_window(wd);
	wd.native_window_handle = native_widow;
	wd.window_object = create_window_object(id);

	render_target_id vp_id = renderer_cache::render_target_cache()->create_render_target(render_target_cache_module::VIEWPORT, wd.rect.get_size());
	wd.viewport = vp_id;

	renderer_cache::render_target_cache()->set_viewport_parent_window(vp_id, wd.id);
	
	return wd;
}

const display_manager::window_data& windows_display_manager::get_main_window()
{
	return get_window_data(MAIN_WINDOW_ID);
}

const display_manager::window_data& windows_display_manager::get_window_data(window_id window)
{
	return m_window_data_map_[window];
}

GLFWwindow* windows_display_manager::create_glfw_window(const window_data& window_data)
{
	glfwDefaultWindowHints();

	GLFWwindow* window = nullptr;
	GLFWwindow* parent_window = nullptr;

	if (window_data.parent_id != INVALID_WINDOW_ID)
	{
		parent_window = m_window_data_map_[window_data.parent_id].native_window_handle;
	}
	else if(window_data.id != MAIN_WINDOW_ID)
	{
		parent_window = m_window_data_map_[MAIN_WINDOW_ID].native_window_handle;
	}
		
	monitor_data& md = m_monitor_data_map_[window_data.monitor_id];
	const GLFWvidmode* current_vid_mode = glfwGetVideoMode(md.native_monitor);

	glfwWindowHint(GLFW_RESIZABLE, window_data.is_resizable ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_FLOATING, window_data.is_always_on_top ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_DECORATED, (window_data.border_style == bordered) ? GLFW_TRUE : GLFW_FALSE);

#if defined(HND_DEBUG) && defined(HND_RENDERER_OPEN_GL)
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

	
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
	
	m_native_window_id_map_[window] = window_data.id;
	
	return window;
}

void windows_display_manager::destroy_glfw_window(window_id id)
{
	const glfw_window_data& data = m_window_data_map_[id];
	
	glfwDestroyWindow(data.native_window_handle);

	m_native_window_id_map_.erase(data.native_window_handle);
}

void windows_display_manager::window_set_title(window_id window, const std::string& title)
{
	GLFWwindow* native_window = m_window_data_map_[window].native_window_handle;
	glfwSetWindowTitle(native_window, title.c_str());
}

void windows_display_manager::window_set_rect(window_id window, const rect_i& rect)
{
	GLFWwindow* native_window = m_window_data_map_[window].native_window_handle;
	glfwSetWindowPos(native_window, rect.get_x(), rect.get_y());
	glfwSetWindowSize(native_window, rect.get_width(), rect.get_height());
}

void windows_display_manager::window_set_frame_buffer_rect(window_id window, const rect_i& rect)
{
	
}

void windows_display_manager::window_set_min_size(window_id window, const vec2_i& min_size)
{
	const window_data& md = get_window_data(window);
	GLFWwindow* native_window = m_window_data_map_[window].native_window_handle;
	glfwSetWindowSizeLimits(native_window, min_size.get_x(), min_size.get_y(), md.max_size.get_x(), md.max_size.get_y());
}

void windows_display_manager::window_set_max_size(window_id window, const vec2_i& max_size)
{
	const window_data& md = get_window_data(window);
	GLFWwindow* native_window = m_window_data_map_[window].native_window_handle;
	glfwSetWindowSizeLimits(native_window, md.max_size.get_x(), md.max_size.get_y(), max_size.get_x(), max_size.get_y());
}

void windows_display_manager::window_set_mode(window_id window, window_mode mode)
{
	GLFWwindow* native_window = m_window_data_map_[window].native_window_handle;
	const window_data& wd = get_window_data(window);

	monitor_data& md = m_monitor_data_map_[wd.monitor_id];

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

void windows_display_manager::window_set_visible(window_id window, bool visible)
{
	GLFWwindow* native_window = m_window_data_map_[window].native_window_handle;
	if (visible)
	{
		glfwShowWindow(native_window);
	}
	else
	{
		glfwHideWindow(native_window);
	}
}

void windows_display_manager::window_set_focused(window_id window, bool focused)
{
	GLFWwindow* native_window = m_window_data_map_[window].native_window_handle;
	if (focused)
	{
		glfwFocusWindow(native_window);
	}
}

void windows_display_manager::window_set_resizable(window_id window, bool is_resizable)
{
	GLFWwindow* native_window = m_window_data_map_[window].native_window_handle;
	glfwSetWindowAttrib(native_window, GLFW_RESIZABLE, is_resizable ? GLFW_TRUE : GLFW_FALSE);
}

void windows_display_manager::window_set_content_scale(window_id window, const vec2_f& scale)
{
}

void windows_display_manager::window_set_aspect(window_id window, const vec2_i& aspect)
{
	GLFWwindow* native_window = m_window_data_map_[window].native_window_handle;
	glfwSetWindowAspectRatio(native_window, aspect.get_x(), aspect.get_y());
}

void windows_display_manager::window_set_maximized(window_id window, bool maximized)
{
	GLFWwindow* native_window = m_window_data_map_[window].native_window_handle;
	
	if(glfwGetWindowAttrib(native_window, GLFW_MAXIMIZED) != (maximized ? GLFW_TRUE : GLFW_FALSE))
		glfwMaximizeWindow(native_window);
}

void windows_display_manager::window_set_minimized(window_id window, bool minimized)
{
	GLFWwindow* native_window = m_window_data_map_[window].native_window_handle;
	
	if(glfwGetWindowAttrib(native_window, GLFW_ICONIFIED) != (minimized ? GLFW_TRUE : GLFW_FALSE))
		glfwIconifyWindow(native_window);
}

void windows_display_manager::window_set_is_always_on_top(window_id window, bool is_always_on_top)
{
	GLFWwindow* native_window = m_window_data_map_[window].native_window_handle;

	if(glfwGetWindowAttrib(native_window, GLFW_FLOATING) != (is_always_on_top ? GLFW_TRUE : GLFW_FALSE))
		glfwSetWindowAttrib(native_window, GLFW_FLOATING, is_always_on_top ? GLFW_TRUE : GLFW_FALSE);
}

void windows_display_manager::window_set_border_style(window_id window, window_border_style style)
{
	GLFWwindow* native_window = m_window_data_map_[window].native_window_handle;
	if (glfwGetWindowAttrib(native_window, GLFW_DECORATED) != ((style == bordered) ? GLFW_TRUE : GLFW_FALSE))
		glfwSetWindowAttrib(native_window, GLFW_DECORATED, (style == bordered) ? GLFW_TRUE : GLFW_FALSE);
}

void windows_display_manager::window_request_attention(window_id window)
{
	GLFWwindow* native_window = m_window_data_map_[window].native_window_handle;
	glfwRequestWindowAttention(native_window);
}

rect_i windows_display_manager::monitor_centered_rect(const vec2_i& size, monitor_id monitor)
{
	const monitor_data& md = m_monitor_data_map_[monitor];

	const rect_i& monitor_size = md.work_area;

	return monitor_size.get_centered_rect_for_size(size);
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

void initialize_display_manager()
{
	engine::register_singleton<display_manager, windows_display_manager>();
}