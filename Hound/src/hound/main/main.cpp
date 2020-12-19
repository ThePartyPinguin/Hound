#include "hound/hnd_pch.h"
#include "main.h"

#include "hound/config/engine.h"
#include "hound/core/window/window.h"
#include "hound/core/input/input_system.h"
#include "hound/drivers/graphics_context.h"
#include "hound/file/shader/shader_file_handler.h"

#include "hound/core/rendering/renderer.h"
#include "hound/core/rendering/renderer_cache.h"

// const char* vertexShaderSource = "#version 330 core\n"
// "layout (location = 0) in vec3 aPos;\n"
// "void main()\n"
// "{\n"
// "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
// "}\0";
// const char* fragmentShaderSource = "#version 330 core\n"
// "out vec4 FragColor;\n"
// "void main()\n"
// "{\n"
// "   FragColor = vec4(1.0f, 1.0f, 0.2f, 1.0f);\n"
// "}\n\0";


ref<application> main::s_application_ = nullptr;

error main::setup(const char* exec_path, int argc, char* argv[])
{	
	logger::init();
	engine::init();
	engine::register_singleton<input_system>();

	application::s_startup_path_ = std::string(exec_path);
	
	return error::OK;
}

bool main::start(ref<application>& p_application)
{
	s_application_ = p_application;
	graphics_context::initialize();


	//Create main window viewport
	window* main_window = display_driver::get_instance()->get_window_handle(display_driver::MAIN_WINDOW_ID);
	renderer_cache::render_target_cache()->create_render_target(render_target_cache_module::VIEWPORT, main_window->get_rect().get_size());
	
	return true;
}

void main::run()
{
	s_application_->init();
	
	mesh_cache_module* mesh_cache = renderer_cache::mesh_cache();
	shader_cache_module* shader_cache = renderer_cache::shader_cache();
	const object_id mesh = mesh_cache->mesh_create();
	
	const mesh_cache_module::mesh_data data
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
	
	mesh_cache->mesh_add_data(mesh, data);
	
	object_id s_id = shader_cache->shader_create_from_absolute_file(R"(F:\SilverWolf\Test\Created\FlatShader.shad)");

	renderer::get_instance()->set_clear_color({ 0.2f, 0.2f, 0.2f, 1.0 });
	
	while(true)
	{
		//Capture window events
		display_driver::get_instance()->process_window_events();

		//Process any inputs captured during the window events
		input_system::get_instance()->process_input_events();

		std::set<render_target_id> targets = renderer_cache::render_target_cache()->get_render_targets();

		for (const render_target_id target : targets)
		{
			renderer::get_instance()->begin_frame(target);

			renderer::get_instance()->render_indexed(s_id, mesh);

			renderer::get_instance()->end_frame(target);

			if(renderer_cache::render_target_cache()->viewport_has_parent_window(target))
			{
				const window_id window = renderer_cache::render_target_cache()->get_viewport_parent_window(target);
				display_driver::get_instance()->redraw_window(window);
			}
		}
	}
}

void main::clean_up()
{
	engine::clean_up();
}
