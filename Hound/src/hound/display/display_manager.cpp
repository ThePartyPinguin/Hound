#include "hound/hnd_pch.h"
#include "display_manager.h"
#include "hound/core/object/object_database.h"
#include "hound/core/window/window.h"
#include "hound/core/input/input_system.h"
#include "hound/core/rendering/renderer_cache.h"
#include "hound/core/rendering/renderer_cache/module/frame_buffer_cache_module.h"
#include "hound/core/rendering/renderer_cache/module/render_target_cache_module.h"

display_manager* display_manager::s_instance_ = nullptr;

display_manager::display_manager()
{
}

display_manager::~display_manager()
{
}

bool display_manager::destroy_sub_window(window_id id)
{
	return true;
}

window* display_manager::create_window_object(window_id window_id)
{
	auto* w = object_database::get_instance()->create_object_instance<window>();
	w->m_window_id_ = window_id;
	return w;
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

void display_manager::set_window_frame_buffer(window_id window_id, const rect_i& rect)
{
	const auto window_data = get_window_data(window_id);
	
	renderer_cache::render_target_cache()->render_target_set_frame_buffer_size(window_data.viewport, rect.get_size());
	window_data.window_object->dpm_set_frame_buffer_rect(rect);
}

void display_manager::set_window_aspect(window_id window_id, const vec2_i& aspect)
{
	const auto window_data = get_window_data(window_id);
	window_data.window_object->dpm_set_aspect(aspect);
}

void display_manager::set_window_content_scale(window_id window_id, const vec2_f& scale)
{
	const auto window_data = get_window_data(window_id);
	window_data.window_object->dpm_set_content_scale(scale);
}

