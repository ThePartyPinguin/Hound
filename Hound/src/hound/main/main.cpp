#include "hound/hnd_pch.h"
#include "main.h"

#include "hound/config/engine.h"
#include "hound/core/scene/render_target/window/window.h"
#include "hound/managers/display_manager.h"
#include "hound/core/input/input_system.h"
#include "hound/drivers/graphics_context.h"

#include "hound/rendering/renderer.h"

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";


ref<application> main::s_application_ = nullptr;

error main::setup(const char* exec_path, int argc, char* argv[])
{	
	logger::init();
	engine::init();
	engine::register_singleton<input_system>();
	
	return error::OK;
}

bool main::start(ref<application>& p_application)
{
	s_application_ = p_application;
	graphics_context::initialize();
	return true;
}

void main::run()
{
	s_application_->init();

	const object_id mesh = renderer_cache::get_instance()->mesh_create();
	
	const renderer_cache::mesh_data data
	{
		{
			{ {0.0f,  0.5f, 0.0f}, {}, {}},
			{ {0.5f, -0.5f, 0.0f}, {}, {}},
			{ {-0.5f, -0.5f, 0.0f}, {}, {}},
		},
		{
			0, 1, 2
		}
	};
	
	renderer_cache::get_instance()->mesh_add_data(mesh, data);

	const object_id shader = renderer_cache::get_instance()->shader_create();

	renderer_cache::get_instance()->shader_set_source(shader, renderer_cache::shader_stage::VERTEX, vertexShaderSource);
	renderer_cache::get_instance()->shader_set_source(shader, renderer_cache::shader_stage::FRAGMENT, fragmentShaderSource);

	const bool shader_finished = renderer_cache::get_instance()->shader_finalize(shader);

	if(!shader_finished)
	{
		HND_CORE_LOG_ERROR("Shader compile failed, exiting");
		return;
	}
	
	while(true)
	{
		display_manager::get_instance()->process_all_window_events();

		renderer::get_instance()->begin_frame();

		renderer::get_instance()->render_indexed(shader, mesh);
		
		renderer::get_instance()->end_frame();
		
		display_manager::get_instance()->redraw_windows();
	}
}

void main::clean_up()
{
	engine::clean_up();
}
