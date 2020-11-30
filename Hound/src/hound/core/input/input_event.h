#pragma once
#include "hound/core/base.h"
#include "hound/core/property.h"
#include "hound/core/input/key_codes.h"
#include "hound/core/math/math.h"

class window;

class input_event
{
public:
	HND_PROPERTY(window_id, uint32_t, m_origin_window_id_)
	HND_PROPERTY_PTR(window_object, window, m_origin_window_object_)

	input_event() = default;
	
private:
	uint32_t m_origin_window_id_;	
	window* m_origin_window_object_;	
};

class input_event_with_modifier : public input_event
{
public:
	HND_PROPERTY(modifier_keys, int, m_modifier_keys_)
	HND_PROPERTY(action, key_action, m_action_)

	input_event_with_modifier() = default;
	
private:
	int m_modifier_keys_;
	key_action m_action_;
};

class input_event_key : public input_event_with_modifier
{
public:
	HND_PROPERTY(key, key_code, m_key_)

	input_event_key() = default;
	
private:
	key_code m_key_;
};

class input_event_mouse : public input_event_with_modifier
{
public:
	HND_PROPERTY(mouse_button, key_code, m_mouse_button_)
	HND_PROPERTY(window_pos, vec2_d, m_mouse_window_position_)
	HND_PROPERTY(global_pos, vec2_d, m_mouse_global_position_)
	
	input_event_mouse() = default;
	
private:
	key_code m_mouse_button_;
	vec2_d m_mouse_window_position_;
	vec2_d m_mouse_global_position_;
};

class input_event_mouse_move : public input_event_mouse
{
public:	
	HND_PROPERTY(window_move_delta, vec2_d, m_relative_move_delta_)
	HND_PROPERTY(global_move_delta, vec2_d, m_global_move_delta_)
	
	input_event_mouse_move() = default;
	
private:
	vec2_d m_relative_move_delta_;
	vec2_d m_global_move_delta_;
};

