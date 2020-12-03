#include "hound/hnd_pch.h"
#include "open_gl_context.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "hound/config/engine.h"
#include "hound/managers/display_manager.h"
#include "hound/platform/open_gl/logging/open_gl_logger.h"
#include "hound/platform/open_gl/renderer/open_gl_renderer.h"
#include "hound/platform/open_gl/renderer/open_gl_renderer_cache.h"


open_gl_context::open_gl_context()
{
	s_instance_ = this;
	
	if (glfwGetCurrentContext() == nullptr)
	{
		HND_CORE_LOG_ERROR("Could not initialize OpenGL context, glfw context not set! Probably no window has been created yet!");
		return;
	}

	const int glad_init_status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	if (!glad_init_status)
	{
		HND_CORE_LOG_ERROR("Could not init glad! exit code: ", glad_init_status);
		return;
	}
	HND_CORE_LOG_INFO("OpenGL Info:\n", "  Vendor:\t", glGetString(GL_VENDOR), "\n  Renderer:\t", glGetString(GL_RENDERER), "\n  Version:\t", glGetString(GL_VERSION));

	open_gl_logger::init();
	s_glad_initialized = true;

	const display_manager::window_id main_window_id = display_manager::get_instance()->get_main_window();
	const renderer_cache::frame_buffer_id frame_buffer = renderer_cache::get_instance()->frame_buffer_create();

	display_manager::get_instance()->window_bind_frame_buffer(main_window_id, frame_buffer);
}

open_gl_context::~open_gl_context()
{
	
}

void initialize_graphics_context()
{
	engine::register_singleton<graphics_context, open_gl_context>();
	engine::register_singleton<renderer, open_gl_renderer>();
	engine::register_singleton<renderer_cache, open_gl_renderer_cache>();
}

