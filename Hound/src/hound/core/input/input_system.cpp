#include "hound/hnd_pch.h"
#include "input_system.h"
#include "hound/core/input/input_event.h"
#include "hound/core/scene/render_target/window/window.h"

input_system* input_system::s_instance_ = nullptr;

input_system::input_system()
{
	s_instance_ = this;
}

void input_system::publish_key_event(window* window, uint32_t window_id, key_code key, key_action action, int modifiers)
{
	input_event_key e{};
	e.set_window_id(window_id);
	e.set_window_object(window);
	e.set_modifier_keys(modifiers);
	e.set_action(action);
	e.set_key(key);

	publish_event(e);
}

void input_system::publish_char_input_event(window* window, uint32_t window_id)
{
	HND_CORE_LOG_TRACE("Char input event not implemented yet!");
}

void input_system::publish_mouse_position_event(window* window, uint32_t window_id, double pos_x, double pos_y)
{
	const vec2_d window_mouse_pos(pos_x, pos_y);
	const vec2_d window_mouse_delta = window_mouse_pos - m_mouse_data_.window_mouse_position;
	m_mouse_data_.window_mouse_position = window_mouse_pos;

	const vec2_i window_origin = window->get_rect().get_origin();
	const vec2_d global_mouse_pos = window_mouse_pos + vec2_d(window_origin.get_x(), window_origin.get_y());

	const vec2_d global_mouse_delta = global_mouse_pos - m_mouse_data_.global_mouse_position;
	m_mouse_data_.global_mouse_position = global_mouse_pos;
	
	input_event_mouse_move e{};
	e.set_window_id(window_id);
	e.set_window_object(window);
	e.set_modifier_keys(m_mouse_data_.mods);
	e.set_action(m_mouse_data_.button_action);
	e.set_mouse_button(m_mouse_data_.button);
	
	e.set_window_pos(window_mouse_pos);
	e.set_window_move_delta(window_mouse_delta);
	e.set_global_pos(global_mouse_pos);
	e.set_global_move_delta(global_mouse_delta);

	publish_event(e);
}

void input_system::publish_mouse_button_event(window* window, uint32_t window_id, key_code button, key_action action, int modifiers)
{
	m_mouse_data_.button_action = action;
	m_mouse_data_.button = button;
	m_mouse_data_.mods = modifiers;

	input_event_mouse e{};
	e.set_window_id(window_id);
	e.set_window_object(window);
	e.set_modifier_keys(m_mouse_data_.mods);
	e.set_action(m_mouse_data_.button_action);
	e.set_mouse_button(m_mouse_data_.button);

	e.set_global_pos(m_mouse_data_.global_mouse_position);
	e.set_window_pos(m_mouse_data_.window_mouse_position);

	publish_event(e);

	if (action == key_action::RELEASED)
		m_mouse_data_.button = key_code::NONE;
}
