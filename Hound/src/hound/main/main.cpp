#include "hound/hnd_pch.h"
#include "main.h"

#include "../../../camera_manager.h"
#include "hound/config/engine.h"
#include "hound/core/scene/render_target/window/window.h"
#include "hound/managers/display_manager.h"
#include "hound/core/input/input_system.h"

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
	
	engine::register_singleton<camera_manager>();

	camera_manager* cam_manager = camera_manager::get_singleton();	
		
	return true;
}

void main::run()
{
	s_application_->init();
		
	while(true)
	{
		display_manager::get_instance()->process_all_window_events();

		display_manager::get_instance()->redraw_windows();
	}
}

void main::clean_up()
{
	engine::clean_up();
}
