#include "hound/hnd_pch.h"
#include "glfw_display_driver.h"


#include "glfw_window.h"
#include "hound/platform/glfw/glfw_callback_handler.h"
#include "GLFW/glfw3.h"
#include "hound/core/object/object_database.h"

display_driver* display_driver::s_instance = nullptr;

glfw_display_driver::glfw_display_driver()
{
	s_instance = this;
	
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
}

window_id glfw_display_driver::create_window(const char* title, const vec2_i& size, window_id parent_window_id)
{
	glfw_window_data& data = create_new_window_data();

	glfw_window* window_handle = object_database::get_instance()->create_object_instance<glfw_window>();
	data.object_handle = window_handle;
	data.native_window_handle = create_native_window(title, size, parent_window_id);
	data.parent = parent_window_id;

	window_handle->init(data.id, title, data.native_window_handle);
	
	get_window_data(parent_window_id).children.insert(data.id);

	glfwSetWindowUserPointer(data.native_window_handle, data.object_handle);
	
	return data.id;
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
	return glfwGetProcAddress;
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
