#include "hound/hnd_pch.h"
#include "main.h"

#include "hound/config/engine.h"
#include "hound/core/window/window.h"
#include "hound/display/display_manager.h"
#include "hound/core/input/input_system.h"
#include "hound/drivers/graphics_context.h"
#include "hound/file/shader/shader_file_handler.h"

#include "hound/core/rendering/renderer.h"
#include "hound/core/rendering/renderer_cache.h"

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

	application::s_startup_path_ = std::string(exec_path);
	
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

	const object_id default_shader = shader_cache->shader_create();

	shader_cache->shader_set_source(default_shader, shader::stage::VERTEX, vertexShaderSource);
	shader_cache->shader_set_source(default_shader, shader::stage::FRAGMENT, fragmentShaderSource);

	const bool shader_finished = shader_cache->shader_finalize(default_shader);

	shader_file_handler handler(".shad");
	
	object_id s_id = handler.load_from_absolute_path("F:\\SilverWolf\\Test\\Created\\FlatShader.shad");

	// shader* compiled_shader = object_database::get_instance()->get_object_instance<shader>(s_id);

	
	if(!shader_finished)
	{
		HND_CORE_LOG_ERROR("Shader compile failed, exiting");
		return;
	}

	renderer::get_instance()->set_clear_color({ 0.2, 0.2, 0.2, 1.0 });
	
	while(true)
	{
		display_manager::get_instance()->process_window_events();

		std::set<render_target_id> targets = renderer_cache::render_target_cache()->get_render_targets();

		for (const render_target_id target : targets)
		{
			renderer::get_instance()->begin_frame(target);

			renderer::get_instance()->render_indexed(s_id, mesh);

			renderer::get_instance()->end_frame(target);

			display_manager::get_instance()->redraw_windows();
		}
	}
}

void main::clean_up()
{
	engine::clean_up();
}
