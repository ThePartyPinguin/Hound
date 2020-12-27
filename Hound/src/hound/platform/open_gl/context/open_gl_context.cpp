#include "hound/hnd_pch.h"
#include "open_gl_context.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "hound/config/engine.h"
#include "hound/core/rendering/renderer_cache/module/frame_buffer_cache_module.h"
#include "hound/drivers/display_driver.h"
#include "hound/platform/open_gl/logging/open_gl_logger.h"
#include "hound/platform/open_gl/renderer/open_gl_renderer.h"
#include "hound/platform/open_gl/renderer/renderer_cache/open_gl_renderer_cache.h"


window_id open_gl_context::request_window()
{
	return 0;
}

void open_gl_context::set_window_context_active(GLFWwindow* window)
{
	glfwMakeContextCurrent(window);
}

open_gl_context::open_gl_context()
{
	s_instance_ = this;

	const int glad_init_status = gladLoadGLLoader((GLADloadproc)display_driver::get_instance()->get_native_proc_address());

	if (!glad_init_status)
	{
		HND_CORE_LOG_ERROR("Could not init glad! exit code: ", glad_init_status);
		return;
	}
	HND_CORE_LOG_INFO("OpenGL Info:\n", "  Vendor:\t", glGetString(GL_VENDOR), "\n  Renderer:\t", glGetString(GL_RENDERER), "\n  Version:\t", glGetString(GL_VERSION));
	open_gl_logger::init();
	
	s_glad_initialized = true;
}

open_gl_context::~open_gl_context()
{
	
}

void initialize_graphics_context()
{
	engine::register_singleton<graphics_context, open_gl_context>();
	open_gl_renderer_cache* cache = engine::register_singleton<renderer_cache, open_gl_renderer_cache>();
	cache->init();
	engine::register_singleton<renderer, open_gl_renderer>();
}

