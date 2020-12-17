#pragma once
#include <string>
#include "hound/core/input/input_event.h"

#include "hound/core/property.h"
#include "hound/core/math/math.h"

class window_close_event : public input_event
{
public:
	HND_PROPERTY(is_main_window, bool, m_is_main_window_)
	HND_PROPERTY(should_close, bool, m_should_close_)
	
	window_close_event() = default;
private:
	bool m_is_main_window_;
	bool m_should_close_;
};

class window_resize_event : public input_event
{
public:
	HND_PROPERTY(rect, rect_i, m_rect_)

	window_resize_event() = default;
private:
	rect_i m_rect_;
};

class window_maximize_event : public input_event
{
public:
	HND_PROPERTY(is_maximized, bool, m_is_maximized_)
	
	window_maximize_event() = default;
private:
	bool m_is_maximized_;
};

class window_minimize_event : public input_event
{
public:
	HND_PROPERTY(is_minimized, bool, m_is_minimized_)

		window_minimize_event() = default;
private:
	bool m_is_minimized_;
};

class window_frame_buffer_resize_event : public input_event
{
public:
	HND_PROPERTY(rect, rect_i, m_rect_)

	window_frame_buffer_resize_event() = default;
private:
	rect_i m_rect_;
};

class window_content_scale_change_event : public input_event
{
public:
	HND_PROPERTY(scale, vec2_f, m_scale_)

	window_content_scale_change_event() = default;
private:
	vec2_f m_scale_;
};

class window_move_event : public input_event
{
public:
	HND_PROPERTY(rect, rect_i, m_rect_)

		window_move_event() = default;
private:
	rect_i m_rect_;
};

class window_focused_event : public input_event
{
public:
	HND_PROPERTY(is_focused, bool, m_is_focused_)

		window_focused_event() = default;
private:
	bool m_is_focused_;
};

class window_mouse_move_event : public input_event
{
public:
	HND_PROPERTY(mouse_pos, vec2_d, m_mouse_pos_)

	window_mouse_move_event() = default;
private:
	vec2_d m_mouse_pos_;
};

class window_mouse_button_input_event : public input_event
{
public:
	HND_PROPERTY(button, key_code, m_button_)
	HND_PROPERTY(action, key_action, m_action_)
	HND_PROPERTY(mods, int, m_mods_)

	window_mouse_button_input_event() = default;
private:
	key_code m_button_;
	key_action m_action_;
	int m_mods_;
};

class window_key_input_event : public input_event
{
public:
	HND_PROPERTY(button, key_code, m_key_)
	HND_PROPERTY(action, key_action, m_action_)
	HND_PROPERTY(mods, int, m_mods_)

	window_key_input_event() = default;
private:
	key_code m_key_;
	key_action m_action_;
	int m_mods_;
};
