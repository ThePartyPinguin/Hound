#include "hound/hnd_pch.h"
#include "main.h"

#include "hound/config/engine.h"
#include "hound/core/window/window.h"
#include "hound/core/input/input_system.h"
#include "hound/drivers/graphics_context.h"

#include "hound/core/rendering/renderer.h"
#include "hound/core/rendering/target/render_target.h"
#include "hound/core/rendering/target/viewport.h"

#include "hound/core/object/mesh/mesh.h"
#include "hound/core/object/mesh/mesh_surface_data.h"
#include "hound/core/os/os.h"
#include "hound/core/io/file/file_handle.h"
#include "hound/core/rendering/renderer_cache/renderer_cache.h"
#include "hound/core/object/shader/shader.h"

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

	const vec2_i& s = main_window->get_rect().get_size();
	
	viewport* vp = viewport::create(s);
	vp->set_owner_window(main_window);
	display_driver::get_instance()->set_window_viewport(main_window->get_window_id(), vp);
	
	os::get_instance()->m_main_window_ = main_window;
	return true;
}

void main::run()
{
	s_application_->init();
	
	mesh* mesh = mesh::create();
	
	const mesh_surface_data data
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
	
	mesh->add_surface_data(data);

	shader* shader_instance = shader::create_from_absolute_path(R"(F:\SilverWolf\Test\Created\FlatShader.shad)");
	
	renderer::get_instance()->set_clear_color({ 0.2f, 0.2f, 0.2f, 1.0 });
	
	while(true)
	{
		//Capture window events
		display_driver::get_instance()->process_window_events();

		//Process any inputs captured during the window events
		input_system::get_instance()->process_input_events();

		
		std::set<render_target*> targets = renderer_cache::render_target_cache()->get_render_targets();

		for (render_target* target : targets)
		{
			renderer::get_instance()->begin_frame(target);

			// renderer::get_instance()->render_indexed(shader_instance->get_object_id(), mesh->get_object_id());

			renderer::get_instance()->end_frame(target);

			if(target->get_type() == render_target::VIEWPORT)
			{
				viewport* vp = static_cast<viewport*>(target);
				window* window = vp->get_owner_window();
				display_driver::get_instance()->redraw_window(window->get_window_id());
			}
		}
	}
}

void main::clean_up()
{
	engine::clean_up();
}
