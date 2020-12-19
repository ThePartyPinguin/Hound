#include "hound/hnd_pch.h"
#include "glfw_display_driver.h"



#include "glfw_monitor.h"
#include "glfw_window.h"
#include "hound/platform/glfw/glfw_callback_handler.h"
#include "GLFW/glfw3.h"
#include "hound/core/input/input_system.h"
#include "hound/core/object/object_database.h"
#include "hound/core/window/monitor.h"
#include "hound/core/rendering/target/viewport.h"

display_driver* display_driver::s_instance_ = nullptr;

glfw_display_driver::glfw_display_driver()
{
	s_instance_ = this;
}

glfw_display_driver::~glfw_display_driver()
{
}

void glfw_display_driver::init()
{
	if(!glfwInit())
	{
		HND_CORE_LOG_ERROR("Could not initialize GLFW display driver!");
		return;
	}
	HND_CORE_LOG_INFO("GLFW display driver initialized!");

	identify_monitors();

	//Create main window
	const window_id main_window = create_window("Main window", { 1280, 720 }, INVALID_WINDOW_ID, MAIN_MONITOR_ID);
	set_window_active_context(main_window);
}

window_id glfw_display_driver::create_window(const char* title, const vec2_i& size, window_id parent_window_id, monitor_id monitor_id)
{
	glfw_monitor_data& monitor_data = m_monitor_data_map_[monitor_id];
	glfw_window_data& data = create_new_window_data();

	glfw_window* window_handle = object_database::get_instance()->create_object_instance<glfw_window>();
	data.object_handle = window_handle;
	data.native_window_handle = create_native_window(title, size, parent_window_id);
	data.parent = parent_window_id;

	add_window_to_monitor(data.id, monitor_data.id);
	
	window_handle->init(data.id, title, data.native_window_handle);
	
	get_window_data(parent_window_id).children.insert(data.id);

	glfwSetWindowUserPointer(data.native_window_handle, data.object_handle);

	monitor_data.containing_windows.insert(data.id);
	
	return data.id;
}

void glfw_display_driver::set_window_viewport(window_id window, viewport* viewport)
{
	if(m_window_data_map_.count(window))
	{
		glfw_window* w_ptr = static_cast<glfw_window*>(m_window_data_map_[window].object_handle);
		w_ptr->m_viewport_ = viewport;
		viewport->set_owner_window(w_ptr);
	}
}

void glfw_display_driver::destroy_window(window_id window)
{
	if (!m_window_data_map_.count(window))
	{
		HND_CORE_LOG_WARN("Window does not exist, could not destory!");
		return;
	}

	destroy_native_window(window);
	m_window_data_map_.erase(window);
}

window* glfw_display_driver::get_window_handle(window_id window)
{
	if (!m_window_data_map_.count(window))
	{
		HND_CORE_LOG_WARN("Window does not exist, could not get window handle. Returning nullptr");
		return nullptr;
	}
	return m_window_data_map_[window].object_handle;
}

void* glfw_display_driver::get_native_window_handle(window_id window)
{
	if(!m_window_data_map_.count(window))
	{
		HND_CORE_LOG_WARN("Window does not exist, could not get native handle. Returning nullptr");
		return nullptr;
	}
	return m_window_data_map_[window].native_window_handle;
}

void* glfw_display_driver::get_native_proc_address()
{
	return static_cast<void*>(glfwGetProcAddress);
}

void glfw_display_driver::redraw_window(window_id window)
{
	if (!m_window_data_map_.count(window))
	{
		HND_CORE_LOG_WARN("Window does not exist, could not redraw!");
		return;
	}

	glfwSwapBuffers(m_window_data_map_[window].native_window_handle);
}

void glfw_display_driver::process_window_events()
{
	glfwPollEvents();
}

glfw_display_driver::glfw_window_data& glfw_display_driver::get_window_data(window_id window)
{
	return m_window_data_map_[window];
}

glfw_display_driver::glfw_window_data& glfw_display_driver::create_new_window_data()
{
	auto& data = m_window_data_map_[m_window_counter_];
	data.id = m_window_counter_;
	++m_window_counter_;
	return data;
}

GLFWwindow* glfw_display_driver::create_native_window(const char* title, const vec2_i& size, window_id parent_window_id)
{
	glfwDefaultWindowHints();

	GLFWwindow* native_window = nullptr;
	GLFWwindow* native_window_parent = nullptr;
	native_window_parent = get_window_data(parent_window_id).native_window_handle;

#if defined(HND_DEBUG) && defined(HND_DISPLAY_GLFW)
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

	native_window = glfwCreateWindow(size.get_x(), size.get_y(), title, nullptr, native_window_parent);

	glfwSetWindowCloseCallback(native_window, glfw_callback_handler::glfw_callback_close);
	glfwSetWindowSizeCallback(native_window, glfw_callback_handler::glfw_callback_size);
	glfwSetFramebufferSizeCallback(native_window, glfw_callback_handler::glfw_callback_frame_buffer_size);
	glfwSetWindowContentScaleCallback(native_window, glfw_callback_handler::glfw_callback_content_scale);
	glfwSetWindowPosCallback(native_window, glfw_callback_handler::glfw_callback_position);
	glfwSetWindowIconifyCallback(native_window, glfw_callback_handler::glfw_callback_iconified);
	glfwSetWindowMaximizeCallback(native_window, glfw_callback_handler::glfw_callback_maximized);
	glfwSetWindowFocusCallback(native_window, glfw_callback_handler::glfw_callback_focus);
	glfwSetKeyCallback(native_window, glfw_callback_handler::glfw_callback_key_input);
	glfwSetCharCallback(native_window, glfw_callback_handler::glfw_callback_char_input);
	glfwSetInputMode(native_window, GLFW_LOCK_KEY_MODS, GLFW_TRUE);

	return native_window;
}

void glfw_display_driver::destroy_native_window(window_id window)
{
	glfwDestroyWindow(m_window_data_map_[window].native_window_handle);
}

void glfw_display_driver::native_request_attention(window_id window)
{
	glfwRequestWindowAttention(get_window_data(window).native_window_handle);
}

void glfw_display_driver::set_native_title(window_id window, const char* title)
{
	glfwSetWindowTitle(get_window_data(window).native_window_handle, title);
}

void glfw_display_driver::set_native_rect(window_id window, const rect_i& rect)
{
	glfwSetWindowPos(get_window_data(window).native_window_handle, rect.get_x(), rect.get_y());
	glfwSetWindowSize(get_window_data(window).native_window_handle, rect.get_width(), rect.get_height());
}

void glfw_display_driver::set_native_min_size(window_id window, const vec2_i& size)
{
	const glfw_window_data& data = get_window_data(window);

	const vec2_i& max_size = data.object_handle->get_max_size();
	
	glfwSetWindowSizeLimits(
		get_window_data(window).native_window_handle, 
		size.get_x(),
		size.get_y(),
		max_size.get_x(),
		max_size.get_y()
	);
}

void glfw_display_driver::set_native_max_size(window_id window, const vec2_i& size)
{
	const glfw_window_data& data = get_window_data(window);

	const vec2_i& min_size = data.object_handle->get_min_size();

	glfwSetWindowSizeLimits(
		get_window_data(window).native_window_handle,
		min_size.get_x(),
		min_size.get_y(),
		size.get_x(),
		size.get_y()
	);
}

void glfw_display_driver::set_native_aspect(window_id window, const vec2_i& aspect)
{
	glfwSetWindowAspectRatio(get_window_data(window).native_window_handle, aspect.get_x(), aspect.get_y());
}

void glfw_display_driver::set_native_mode(window_id window, window_mode aspect)
{
	
}

void glfw_display_driver::set_native_should_close(window_id window, bool should_close)
{
	glfwSetWindowShouldClose(get_window_data(window).native_window_handle, should_close);
}

void glfw_display_driver::set_native_resizable(window_id window, bool is_resizable)
{
	glfwSetWindowAttrib(get_window_data(window).native_window_handle, GLFW_RESIZABLE, is_resizable);
}

void glfw_display_driver::set_native_visible(window_id window, bool is_visible)
{
	if(is_visible)
	{
		glfwShowWindow(get_window_data(window).native_window_handle);
	}
	else
	{
		glfwHideWindow(get_window_data(window).native_window_handle);
	}
}

void glfw_display_driver::set_native_focus(window_id window, bool is_focused)
{
	if(is_focused)
		glfwFocusWindow(get_window_data(window).native_window_handle);
}

void glfw_display_driver::set_native_maximized(window_id window, bool is_maximized)
{
	if(is_maximized)
	{
		glfwMaximizeWindow(get_window_data(window).native_window_handle);
	}
	else
	{
		glfwRestoreWindow(get_window_data(window).native_window_handle);
	}
}

void glfw_display_driver::set_native_minimized(window_id window, bool is_minimized)
{
	if(is_minimized)
	{
		glfwIconifyWindow(get_window_data(window).native_window_handle);
	}
	else
	{
		glfwRestoreWindow(get_window_data(window).native_window_handle);
	}
}

void glfw_display_driver::set_native_always_on_top(window_id window, bool is_always_on_top)
{
	glfwSetWindowAttrib(get_window_data(window).native_window_handle, GLFW_FLOATING, is_always_on_top);
}

void glfw_display_driver::set_native_border_style(window_id window, window_border_style style)
{
	glfwSetWindowAttrib(get_window_data(window).native_window_handle, GLFW_DECORATED, style == window_border_style::bordered);
}

void glfw_display_driver::set_window_active_context(window_id window)
{
	if(!m_window_data_map_.count(window))
	{
		HND_CORE_LOG_WARN("Window id not valid! Not setting active context");
	}

	const glfw_window_data& window_data = m_window_data_map_[window];
	glfwMakeContextCurrent(window_data.native_window_handle);
}

void glfw_display_driver::identify_monitors()
{
	int count;
	GLFWmonitor** monitors = glfwGetMonitors(&count);
	
	for (int i = 0; i < count; ++i)
	{
		GLFWmonitor* native_monitor = monitors[i];

		monitor_id id = m_monitor_counter_;
		
		glfw_monitor_data& data = m_monitor_data_map_[id];
		data.id = id;
		data.native_monitor_handle_ = native_monitor;
		
		glfw_monitor* monitor_object = object_database::get_instance()->create_object_instance<glfw_monitor>();

		monitor_object->init(id, native_monitor);

		data.object_handle = monitor_object;

		++m_monitor_counter_;
	}
}

monitor* glfw_display_driver::get_monitor_handle(monitor_id monitor)
{
	return m_monitor_data_map_[monitor].object_handle;
}

bool glfw_display_driver::is_window_on_monitor(window_id window, monitor_id monitor)
{
	glfw_window* window_handle = dynamic_cast<glfw_window*>(get_window_handle(window));
	glfw_monitor* monitor_handle = dynamic_cast<glfw_monitor*>(get_monitor_handle(monitor));

	return monitor_handle->m_work_area_.contains(window_handle->m_rect_.get_origin());
}

monitor_id glfw_display_driver::get_native_monitor(window_id window_id)
{
	glfw_window* window_handle = dynamic_cast<glfw_window*>(get_window_handle(window_id));

	const vec2_i& position = window_handle->get_rect().get_origin();

	for (auto& pair : m_monitor_data_map_)
	{
		if(pair.first == INVALID_MONITOR_ID)
			continue;
		
		monitor* monitor_handle = pair.second.object_handle;

		const rect_i& monitor_rect = monitor_handle->get_work_area();

		if (monitor_rect.contains(position))
		{
			return pair.first;
		}
	}
	return INVALID_MONITOR_ID;
}

void glfw_display_driver::add_window_to_monitor(window_id window, monitor_id monitor)
{
	m_window_data_map_[window].monitor = monitor;
	dynamic_cast<glfw_window*>(m_window_data_map_[window].object_handle)->m_monitor_id_ = monitor;
	m_monitor_data_map_[monitor].containing_windows.insert(window);
}

void glfw_display_driver::remove_window_from_monitor(window_id window, monitor_id monitor)
{
	m_window_data_map_[window].monitor = INVALID_MONITOR_ID;
	dynamic_cast<glfw_window*>(m_window_data_map_[window].object_handle)->m_monitor_id_ = INVALID_MONITOR_ID;
	m_monitor_data_map_[monitor].containing_windows.erase(window);
}
