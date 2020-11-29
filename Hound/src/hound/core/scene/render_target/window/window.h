#pragma once
#include "hound/core/scene/render_target/viewport.h"
#include "hound/math/math.h"
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
	HND_PROPERTY_READ_ONLY(title, std::string, m_title_)
	HND_PROPERTY_READ_ONLY(rect, rect_i, m_rect_)
	HND_PROPERTY_READ_ONLY(frame_buffer_rect, rect_i, m_frame_buffer_rect_)
	HND_PROPERTY_READ_ONLY(content_scale, vec2_f, m_content_scale_)
	HND_PROPERTY_READ_ONLY(min_size, vec2_i, m_min_size_)
	HND_PROPERTY_READ_ONLY(max_size, vec2_i, m_max_size_)
	HND_PROPERTY_READ_ONLY(aspect, vec2_i, m_aspect_)
	HND_PROPERTY_READ_ONLY(mode, mode, m_mode_)
	
	HND_PROPERTY_READ_ONLY(is_resizable, bool, m_is_resizable_)
	HND_PROPERTY_READ_ONLY(is_visible, bool, m_is_visible_)
	HND_PROPERTY_READ_ONLY(is_focused, bool, m_is_focused_)
	HND_PROPERTY_READ_ONLY(is_maximized, bool, m_is_maximized_)
	HND_PROPERTY_READ_ONLY(is_minimized, bool, m_is_minimized_)
	HND_PROPERTY_READ_ONLY(is_always_on_top, bool, m_is_always_on_top_)
	HND_PROPERTY_READ_ONLY(border_style, border_style, m_border_style_)
	
	HND_PROPERTY_PTR_READ_ONLY(parent, window, m_parent_)
	HND_PROPERTY_READ_ONLY(children, std::set<window*>, m_children_)
	
	static window* create(window* parent = nullptr);
	
	void show();
	void hide();
	void maximize();
	void minimize();
	void grab_focus();
	void set_resizable();
	void resize(const rect_i& rect);
	void resize_frame_buffer(const rect_i& rect);
	void set_content_scale(const rect_i& rect);
	void set_min_size(const vec2_i& min_size);
	void set_max_size(const vec2_i& max_size);
	void request_attention();
	void set_border_style(border_style style);
	void set_always_on_top(bool always_on_top);
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

	bool m_is_resizable_;
	bool m_is_visible_;
	bool m_is_focused_;
	bool m_is_maximized_;
	bool m_is_minimized_;
	bool m_is_always_on_top_;
	border_style m_border_style_;

	window* m_parent_ = nullptr;
	std::set<window *> m_children_;

	static void on_input_event(const input_event_with_modifier& event);
	static void on_window_event(const window_event& event);

	void _set_title(const std::string& title);
	void _set_rect(const rect_i& rect);
	void _set_frame_buffer_rect(const rect_i& rect);
	void _set_content_scale(const vec2_f& scale);
	void _set_min_size(const vec2_i& size);
	void _set_max_size(const vec2_i& size);
	void _set_mode(const mode& mode);
	void _set_flags(const int& flags);
	void _set_visible(const bool& visible);
	void _set_focus(const bool& focus);
};

