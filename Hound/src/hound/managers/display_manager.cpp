#include "hound/hnd_pch.h"
#include "display_manager.h"
#include "hound/core/object/object_database.h"
#include "hound/core/rendering/render_target/window/window.h"
#include "hound/core/input/input_system.h"
#include "hound/core/rendering/renderer_cache.h"

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
	on_init();

	window* main_window_object = object_database::get_instance()->create_object_instance<window>();
	
	const window_data& data = create_window_data(main_window_properties, main_window_object);	
	create_native_window(data);
	attach_window_object(main_window_object, data.id);
	subscribe_to_window_events(main_window_object);
}

display_manager::window_id display_manager::get_main_window()
{
	return MAIN_WINDOW_ID;
}

display_manager::window_id display_manager::request_sub_window(window_id parent_id, const window_properties& properties)
{
	return INVALID_WINDOW_ID;
}

bool display_manager::destroy_sub_window(window_id id)
{
	return true;
}

void display_manager::attach_window_object(window* window_object, window_id window)
{
	m_window_data_[window].window_object = window_object;
	window_object->m_window_id_ = window;
}

window* display_manager::get_attached_window_object(window_id id)
{
	return get_window_data(id).window_object;
}

void display_manager::subscribe_to_window_events(window* window)
{
	window->subscribe<window_close_event>(this);
	window->subscribe<window_visibility_event>(this);
	window->subscribe<window_title_event>(this);
	window->subscribe<window_resize_event>(this);
	window->subscribe<window_maximize_event>(this);
	window->subscribe<window_minimize_event>(this);
	window->subscribe<window_resizable_event>(this);
	window->subscribe<window_frame_buffer_resize_event>(this);
	window->subscribe<window_content_scale_change_event>(this);
	window->subscribe<window_min_size_change_event>(this);
	window->subscribe<window_max_size_change_event>(this);
	window->subscribe<window_aspect_change_event>(this);
	window->subscribe<window_border_style_change_event>(this);
	window->subscribe<window_always_on_top_change_event>(this);
	window->subscribe<window_move_event>(this);
	window->subscribe<window_mode_change_event>(this);
	window->subscribe<window_focused_event>(this);
}

void display_manager::process_all_window_events()
{
	poll_native_window_events();
}

const display_manager::window_data& display_manager::create_window_data(const window_properties& properties, window* window_object)
{
	window_data& wd = m_window_data_[m_window_id_counter_];
	wd.id = m_window_id_counter_;
	wd.title = properties.title;
	wd.rect = properties.rect;
	wd.min_size = properties.min_size;
	wd.max_size = properties.max_size;
	
	wd.mode = properties.mode;
	wd.is_resizable = properties.is_resizable;
	wd.is_visible = properties.is_visible;
	wd.is_focused = properties.is_focused;
	wd.is_maximized = properties.is_maximized;
	wd.is_minimized = properties.is_minimized;
	wd.is_always_on_top = properties.is_always_on_top;
	wd.border_style = properties.border_style;
	wd.monitor_id = properties.monitor_id;
	
	return wd;
}

void display_manager::on_event(const window_close_event& e)
{
	m_window_data_[e.get_window_id()].should_close = e.get_should_close();
}

void display_manager::on_event(const window_visibility_event& e)
{
	m_window_data_[e.get_window_id()].is_visible = e.get_is_visible();
	set_native_window_visible(e.get_window_id(), e.get_is_visible());
}

void display_manager::on_event(const window_title_event& e)
{
	m_window_data_[e.get_window_id()].title = e.get_title();
	set_native_window_title(e.get_window_id(), e.get_title());
}

void display_manager::on_event(const window_resize_event& e)
{
	m_window_data_[e.get_window_id()].rect = e.get_rect();
	set_native_window_rect(e.get_window_id(), e.get_rect());
}

void display_manager::on_event(const window_maximize_event& e)
{
	m_window_data_[e.get_window_id()].is_maximized = e.get_is_maximized();
	set_native_window_maximized(e.get_window_id(), e.get_is_maximized());
}

void display_manager::on_event(const window_minimize_event& e)
{
	m_window_data_[e.get_window_id()].is_minimized = e.get_is_minimized();
	set_native_window_minimized(e.get_window_id(), e.get_is_minimized());
}

void display_manager::on_event(const window_resizable_event& e)
{
	m_window_data_[e.get_window_id()].is_resizable = e.get_is_resizable();
	set_native_window_resizable(e.get_window_id(), e.get_is_resizable());
}

void display_manager::on_event(const window_frame_buffer_resize_event& e)
{
	m_window_data_[e.get_window_id()].frame_buffer_rect = e.get_rect();
	set_native_window_frame_buffer_rect(e.get_window_id(), e.get_rect());
}

void display_manager::on_event(const window_content_scale_change_event& e)
{
	m_window_data_[e.get_window_id()].content_scale = e.get_scale();
	set_native_window_content_scale(e.get_window_id(), e.get_scale());
}

void display_manager::on_event(const window_min_size_change_event& e)
{
	m_window_data_[e.get_window_id()].min_size = e.get_min_size_();
	set_native_window_min_size(e.get_window_id(), e.get_min_size_());
}

void display_manager::on_event(const window_max_size_change_event& e)
{
	m_window_data_[e.get_window_id()].max_size = e.get_max_size_();
	set_native_window_max_size(e.get_window_id(), e.get_max_size_());
}

void display_manager::on_event(const window_aspect_change_event& e)
{
	m_window_data_[e.get_window_id()].aspect = e.get_aspect();
	set_native_window_aspect(e.get_window_id(), e.get_aspect());
}

void display_manager::on_event(const window_border_style_change_event& e)
{
	m_window_data_[e.get_window_id()].border_style = static_cast<window_border_style>(e.get_border_style());
	set_native_window_border_style(e.get_window_id(), static_cast<window_border_style>(e.get_border_style()));
}

void display_manager::on_event(const window_move_event& e)
{
	m_window_data_[e.get_window_id()].rect = e.get_rect();
	set_native_window_rect(e.get_window_id(), e.get_rect());
}

void display_manager::on_event(const window_mode_change_event& e)
{
	m_window_data_[e.get_window_id()].mode = static_cast<window_mode>(e.get_mode());
	set_native_window_mode(e.get_window_id(), static_cast<window_mode>(e.get_mode()));
}

void display_manager::on_event(const window_focused_event& e)
{
	m_window_data_[e.get_window_id()].is_focused = e.get_is_focused();
	set_native_window_focused(e.get_window_id(), e.get_is_focused());
}

void display_manager::on_event(const window_always_on_top_change_event& e)
{
	m_window_data_[e.get_window_id()].is_always_on_top = e.get_always_on_top();
	set_native_window_is_always_on_top(e.get_window_id(), e.get_always_on_top());
}

void display_manager::window_bind_frame_buffer(window_id window, renderer_cache::frame_buffer_id frame_buffer)
{
	if(!m_window_data_.count(window))
	{
		HND_CORE_LOG_ERROR("Could not bind frame buffer to window, window id not found");
		return;
	}

	m_window_data_[window].frame_buffer = frame_buffer;
	renderer_cache::get_instance()->frame_buffer_set_size(frame_buffer, m_window_data_[window].rect.get_size());
}

display_manager::window_properties display_manager::get_default_properties()
{
	window_properties props;

	props.title = "Main window";
	props.rect = { 640, 360, 1280, 720 };
	props.min_size = { 100, 100 };
	props.max_size = { 3840, 2160 };

	props.mode = windowed;
	props.is_resizable = true;
	props.is_visible = true;
	props.is_focused = true;
	props.is_maximized = false;
	props.is_minimized = false;
	props.is_always_on_top = false;
	props.border_style = bordered;

	props.monitor_id = MAIN_MONITOR_ID;
	return props;
}

const display_manager::window_data& display_manager::get_window_data(window_id window)
{
	return m_window_data_[window];
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
	window* window_object = m_window_data_[window_id].window_object;
	window_object->dpm_set_frame_buffer_rect(rect);
}

void display_manager::set_window_aspect(window_id window_id, const vec2_i& aspect)
{
	window* window_object = m_window_data_[window_id].window_object;
	window_object->dpm_set_aspect(aspect);
}

void display_manager::set_window_content_scale(window_id window_id, const vec2_f& scale)
{
	window* window_object = m_window_data_[window_id].window_object;
	window_object->dpm_set_content_scale(scale);
}
