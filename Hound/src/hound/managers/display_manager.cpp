#include "hound/hnd_pch.h"
#include "display_manager.h"
#include "hound/core/object/object_database.h"
#include "hound/core/scene/render_target/window/window.h"
#include "hound/core/input/input_system.h"

display_manager* display_manager::s_instance_ = nullptr;

display_manager::display_manager()
{
	s_instance_ = this;
}

display_manager::~display_manager()
{
}



void display_manager::initialize(const window_properties& main_window_properties, graphics_context* graphics_context)
{
	m_graphics_context_ = graphics_context;
	on_init();

	window* main_window_object = object_database::get_instance()->create_object_instance<window>();
	
	const window_data& data = create_window_data(main_window_properties, main_window_object);	
	create_native_window(data);
}

const display_manager::window_data& display_manager::create_window_data(const window_properties& properties, window* window_object)
{
	window_data& wd = m_window_data_[m_window_id_counter_];
	wd.id = m_window_id_counter_;
	wd.title = properties.title;
	wd.rect = properties.rect;
	wd.mode = properties.mode;
	wd.rect = properties.rect;
	wd.min_size = properties.min_size;
	wd.max_size = properties.max_size;
	wd.aspect = properties.aspect;	
	return wd;
}

display_manager::window_properties display_manager::get_default_properties()
{
	window_properties props;
	props.flags = (window_flag_resizable | window_flag_focused | window_flag_centered);
	props.mode = windowed;
	props.rect = { 0, 0, 1280, 720 };
	props.title = "Main window";
	props.monitor_id = 1;
	props.min_size = { 100, 100 };
	props.max_size = { 3840, 2160 };
	props.aspect = { 16, 9 };
	return props;
}

void display_manager::set_main_window(window* window, window_id id)
{
	window->m_window_id_ = id;
	os::get_instance()->set_main_window(window);
}

void display_manager::pass_key_event(window* window, uint32_t window_id, key_code key, key_action action, int modifiers)
{
	input_system::get_instance()->publish_key_event(window, window_id, key, action, modifiers);
}

void display_manager::pass_mouse_position_event(window* window, uint32_t window_id, double pos_x, double pos_y)
{
	input_system::get_instance()->publish_mouse_position_event(window, window_id, pos_x, pos_y);
}

void display_manager::pass_mouse_button_event(window* window, uint32_t window_id, key_code button, key_action action, int modifiers)
{
	input_system::get_instance()->publish_mouse_button_event(window, window_id, button, action, modifiers);
}
