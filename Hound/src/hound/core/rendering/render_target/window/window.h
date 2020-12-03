#pragma once
#include "hound/core/rendering/render_target/viewport.h"
#include "hound/core/math/math.h"
#include "hound/managers/display_manager.h"
#include "hound/core/event/event.h"

class window : public viewport, public event_publisher
{
public:
	friend class display_manager;
	
	enum class flag
	{
		resizable				= display_manager::window_flag_resizable,
		border_less				= display_manager::window_flag_resizable,
		floating				= display_manager::window_flag_always_on_top,
		focused					= display_manager::window_flag_focused,
		centered				= display_manager::window_flag_centered
	};

	enum class mode
	{
		windowed				= display_manager::windowed,
		minimized				= display_manager::minimized,
		maximized				= display_manager::maximized,
		full_screen				= display_manager::full_screen,
		full_screen_windowed	= display_manager::full_screen_windowed,
	};

	enum class border_style
	{
		border_less				= display_manager::border_less,
		bordered				= display_manager::bordered
	};
	
	HND_PROPERTY_READ_ONLY(window_id, display_manager::window_id, m_window_id_)
	HND_PROPERTY_READ_ONLY(monitor_id, display_manager::monitor_id, m_monitor_id_)
	HND_PROPERTY_CALLBACK(title, std::string, m_title_, on_set_title)
	HND_PROPERTY_CALLBACK(rect, rect_i, m_rect_, on_set_rect)
	HND_PROPERTY_READ_ONLY(frame_buffer_rect, rect_i, m_frame_buffer_rect_)
	HND_PROPERTY_READ_ONLY(content_scale, vec2_f, m_content_scale_)
	HND_PROPERTY_CALLBACK(min_size, vec2_i, m_min_size_, on_set_min_size)
	HND_PROPERTY_CALLBACK(max_size, vec2_i, m_max_size_, on_set_max_size)
	HND_PROPERTY_READ_ONLY(aspect, vec2_i, m_aspect_)
	HND_PROPERTY_CALLBACK(mode, mode, m_mode_, on_set_mode)
	
	HND_PROPERTY_CALLBACK(should_close, bool, m_should_close_, on_set_should_close)
	HND_PROPERTY_CALLBACK(is_resizable, bool, m_is_resizable_, on_set_resizable)
	HND_PROPERTY_CALLBACK(is_visible, bool, m_is_visible_, on_set_visible)
	HND_PROPERTY_CALLBACK(is_focused, bool, m_is_focused_, on_set_focus)
	HND_PROPERTY_CALLBACK(is_maximized, bool, m_is_maximized_, on_set_maximized)
	HND_PROPERTY_CALLBACK(is_minimized, bool, m_is_minimized_, on_set_minimized)
	HND_PROPERTY_CALLBACK(is_always_on_top, bool, m_is_always_on_top_, on_set_always_on_top)
	HND_PROPERTY_CALLBACK(border_style, border_style, m_border_style_, on_set_border_style)
	
	HND_PROPERTY_PTR_READ_ONLY(parent, window, m_parent_)
	HND_PROPERTY_READ_ONLY(children, std::set<window*>, m_children_)
	
	static window* create(window* parent = nullptr);
	
	void show();
	void hide();
	void maximize();
	void minimize();
	void grab_focus();
	void resize(const rect_i& rect);
	void request_attention();
	void close();

	window();
	~window();
private:
	display_manager::window_id m_window_id_ = display_manager::INVALID_WINDOW_ID;
	display_manager::monitor_id m_monitor_id_ = display_manager::MAIN_MONITOR_ID;

	std::string m_title_;
	rect_i m_rect_;
	rect_i m_frame_buffer_rect_;
	
	vec2_f m_content_scale_;
	vec2_i m_min_size_;
	vec2_i m_max_size_;
	vec2_i m_aspect_;
	mode m_mode_;

	bool m_should_close_;
	bool m_is_resizable_;
	bool m_is_visible_;
	bool m_is_focused_;
	bool m_is_maximized_;
	bool m_is_minimized_;
	bool m_is_always_on_top_;
	border_style m_border_style_;

	window* m_parent_ = nullptr;
	std::set<window *> m_children_;

	void dpm_set_frame_buffer_rect(const rect_i& rect);
	void dpm_set_aspect(const vec2_i& aspect);
	void dpm_set_content_scale(const vec2_f& scale);
	
	void on_set_title(const std::string& title);
	void on_set_rect(const rect_i& rect);
	void on_set_min_size(const vec2_i& size);
	void on_set_max_size(const vec2_i& size);
	void on_set_aspect(const vec2_i& aspect);
	void on_set_mode(const mode& mode);
	void on_set_should_close(bool close);
	void on_set_resizable(bool resizable);
	void on_set_visible(const bool& visible);
	void on_set_focus(const bool& focus);
	void on_set_maximized(const bool& is_maximized);
	void on_set_minimized(const bool& is_minimized);
	void on_set_always_on_top(const bool& is_always_on_top);
	void on_set_border_style (const border_style& style);
};

