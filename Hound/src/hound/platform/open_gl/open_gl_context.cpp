#include "hound/hnd_pch.h"
#include "open_gl_context.h"

#include "open_gl_logger.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

void open_gl_context::initialize()
{
	if (!s_glad_initialized)
	{
		if (glfwGetCurrentContext() == nullptr)
		{
			HND_CORE_LOG_ERROR("Could not initialize OpenGL context, glfw context not set!");
		}

		const int glad_init_status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		if(!glad_init_status)
		{
			HND_CORE_LOG_ERROR("Could not init glad! exit code: ", glad_init_status);
			return;
		}
		HND_CORE_LOG_INFO("OpenGL Info:\n", "  Vendor:\t", glGetString(GL_VENDOR), "\n  Renderer:\t", glGetString(GL_RENDERER), "\n  Version:\t", glGetString(GL_VERSION));

		open_gl_logger::init();
		s_glad_initialized = true;
	}
}

open_gl_context::open_gl_context()
{
	s_glad_initialized = false;
}
