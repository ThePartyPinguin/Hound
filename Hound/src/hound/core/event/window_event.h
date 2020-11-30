#pragma once
#include <string>
#include "hound/core/input/input_event.h"

#include "hound/core/property.h"
#include "hound/math/math.h"

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

class window_visibility_event : public input_event
{
public:
	HND_PROPERTY(is_visible, bool, m_is_visible_)

	window_visibility_event() = default;
private:
	bool m_is_visible_;
};

class window_title_event : public input_event
{
public:
	HND_PROPERTY(title, std::string, m_window_title_)

	window_title_event() = default;
private:
	std::string  m_window_title_;
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

class window_resizable_event : public input_event
{
public:
	HND_PROPERTY(is_resizable, bool, m_is_resizable_)
	
	window_resizable_event() = default;
private:
	bool m_is_resizable_;
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

class window_min_size_change_event: public input_event
{
public:
	HND_PROPERTY(min_size_, vec2_i, m_min_size_)

	window_min_size_change_event() = default;
private:
	vec2_i m_min_size_;
};

class window_max_size_change_event : public input_event
{
public:
	HND_PROPERTY(max_size_, vec2_i, m_max_size_)

	window_max_size_change_event() = default;
private:
	vec2_i m_max_size_;
};

class window_aspect_change_event : public input_event
{
public:
	HND_PROPERTY(aspect, vec2_i, m_aspect_)

	window_aspect_change_event() = default;
private:
	vec2_i m_aspect_;
};

class window_border_style_change_event : public input_event
{
public:
	HND_PROPERTY(border_style , int, m_border_style_)

	window_border_style_change_event() = default;
private:
	int m_border_style_;
};

class window_always_on_top_change_event : public input_event
{
public:
	HND_PROPERTY(always_on_top, int, m_is_always_on_top_)

	window_always_on_top_change_event() = default;
private:
	bool m_is_always_on_top_;
};

class window_move_event : public input_event
{
public:
	HND_PROPERTY(rect, rect_i, m_rect_)

		window_move_event() = default;
private:
	rect_i m_rect_;
};

class window_mode_change_event : public input_event
{
public:
	HND_PROPERTY(mode, int, m_mode_)

	window_mode_change_event() = default;
private:
	int m_mode_;
};

class window_focused_event : public input_event
{
public:
	HND_PROPERTY(is_focused, bool, m_is_focused_)

		window_focused_event() = default;
private:
	bool m_is_focused_;
};
