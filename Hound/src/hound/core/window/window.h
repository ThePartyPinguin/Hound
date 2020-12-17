#pragma once
#include "hound/core/math/math.h"
#include "hound/core/event/event.h"
#include "hound/drivers/display_driver.h"
#include "hound/core/window/window_enum.h"

class window : public object, public event_publisher
{
public:
	friend class display_manager;

	
	HND_PROPERTY_READ_ONLY(window_id, window_id, m_window_id_)
	HND_PROPERTY_READ_ONLY(viewport, render_target_id, m_viewport_)

	HND_PROPERTY_READ_ONLY(content_scale, vec2_f, m_content_scale_)
	HND_PROPERTY_CALLBACK(rect, rect_i, m_rect_, on_set_rect)
	HND_PROPERTY_CALLBACK(min_size, vec2_i, m_min_size_, on_set_min_size)
	HND_PROPERTY_CALLBACK(max_size, vec2_i, m_max_size_, on_set_max_size)
	HND_PROPERTY_READ_ONLY(aspect, vec2_i, m_aspect_)
	HND_PROPERTY_CALLBACK(mode, window_mode, m_mode_, on_set_mode)

	HND_PROPERTY_CALLBACK(should_close, bool, m_should_close_, on_set_should_close)
	HND_PROPERTY_CALLBACK(is_resizable, bool, m_is_resizable_, on_set_resizable)
	HND_PROPERTY_CALLBACK(is_visible, bool, m_is_visible_, on_set_visible)
	HND_PROPERTY_CALLBACK(is_focused, bool, m_is_focused_, on_set_focus)
	HND_PROPERTY_CALLBACK(is_maximized, bool, m_is_maximized_, on_set_maximized)
	HND_PROPERTY_CALLBACK(is_minimized, bool, m_is_minimized_, on_set_minimized)
	HND_PROPERTY_CALLBACK(is_always_on_top, bool, m_is_always_on_top_, on_set_always_on_top)
	HND_PROPERTY_CALLBACK(border_style, window_border_style, m_border_style_, on_set_border_style)

	HND_PROPERTY_READ_ONLY(parent, window_id, m_parent_id_)
	HND_PROPERTY_READ_ONLY(children, std::set<window_id>, m_children_)
	
	virtual void show();
	virtual void hide();
	virtual void maximize();
	virtual void minimize();
	virtual void restore();
	virtual void grab_focus();
	virtual void resize(const rect_i& rect);
	virtual void request_attention();
	virtual void close();


	virtual void* get_native_handle() = 0;
	
	window() = default;
	virtual ~window() = default;

protected:
	window_id m_window_id_ = display_driver::INVALID_WINDOW_ID;
	window_id m_parent_id_ = display_driver::INVALID_WINDOW_ID;
	
	std::set<window_id> m_children_;
	std::string m_title_;
	rect_i m_rect_;
	rect_i m_frame_buffer_rect_;

	vec2_f m_content_scale_;
	vec2_i m_min_size_;
	vec2_i m_max_size_;
	vec2_i m_aspect_;

	window_mode m_mode_;
	bool m_should_close_;
	bool m_is_resizable_;
	bool m_is_visible_;
	bool m_is_focused_;
	bool m_is_maximized_;
	bool m_is_minimized_;
	bool m_is_always_on_top_;
	window_border_style m_border_style_;

	render_target_id m_viewport_;
	
	bool check_valid_window() const;

	void log_event_error(const char* error) const;
private:
	void on_set_title(const std::string& title) const;
	void on_set_rect(const rect_i& rect) const;
	void on_set_min_size(const vec2_i& size) const;
	void on_set_max_size(const vec2_i& size) const;
	void on_set_aspect(const vec2_i& aspect) const;
	void on_set_mode(const window_mode& mode) const;
	void on_set_should_close(bool close) const;
	void on_set_resizable(bool resizable) const;
	void on_set_visible(const bool& visible) const;
	void on_set_focus(const bool& focus) const;
	void on_set_maximized(const bool& is_maximized) const;
	void on_set_minimized(const bool& is_minimized) const;
	void on_set_always_on_top(const bool& is_always_on_top) const;
	void on_set_border_style (const window_border_style& style) const;

	void log_set_error(const char* error) const;
};

