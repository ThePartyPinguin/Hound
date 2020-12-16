#pragma once
#include "hound/core/math/math.h"
#include "hound/display/display_manager.h"
#include "hound/core/event/event.h"

#define DPM_I display_manager::get_instance()
#define DPM_WD DPM_I->get_window_data(m_window_id_)

class window : public object, public event_publisher
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
	
	HND_PROPERTY_READ_ONLY(window_id, window_id, DPM_WD.id)
	HND_PROPERTY_READ_ONLY(monitor_id, monitor_id, DPM_WD.monitor_id)
	HND_PROPERTY_READ_ONLY(viewport, render_target_id, DPM_WD.viewport)

	HND_PROPERTY_READ_ONLY(content_scale, vec2_f, DPM_WD.content_scale)
	HND_PROPERTY_CALLBACK(rect, rect_i, DPM_WD.rect, on_set_rect)
	HND_PROPERTY_CALLBACK(min_size, vec2_i, DPM_WD.min_size, on_set_min_size)
	HND_PROPERTY_CALLBACK(max_size, vec2_i, DPM_WD.max_size, on_set_max_size)
	HND_PROPERTY_READ_ONLY(aspect, vec2_i, DPM_WD.aspect)
	HND_PROPERTY_CALLBACK(mode, mode, static_cast<mode>(DPM_WD.mode), on_set_mode)

	HND_PROPERTY_CALLBACK(should_close, bool, DPM_WD.should_close, on_set_should_close)
	HND_PROPERTY_CALLBACK(is_resizable, bool, DPM_WD.is_resizable, on_set_resizable)
	HND_PROPERTY_CALLBACK(is_visible, bool, DPM_WD.is_visible, on_set_visible)
	HND_PROPERTY_CALLBACK(is_focused, bool, DPM_WD.is_focused, on_set_focus)
	HND_PROPERTY_CALLBACK(is_maximized, bool, DPM_WD.is_maximized, on_set_maximized)
	HND_PROPERTY_CALLBACK(is_minimized, bool, DPM_WD.is_minimized, on_set_minimized)
	HND_PROPERTY_CALLBACK(is_always_on_top, bool, DPM_WD.is_always_on_top, on_set_always_on_top)
	HND_PROPERTY_CALLBACK(border_style, border_style, static_cast<border_style>(DPM_WD.border_style), on_set_border_style)

	HND_PROPERTY_READ_ONLY(parent, window_id, DPM_WD.parent_id)
	HND_PROPERTY_READ_ONLY(children, std::set<window_id>, DPM_WD.children)

	static window* create(const std::string& title, vec2_i size, window_id parent_id = display_manager::MAIN_WINDOW_ID);
	
	virtual void show();
	virtual void hide();
	virtual void maximize();
	virtual void minimize();
	virtual void restore();
	virtual void grab_focus();
	virtual void resize(const rect_i& rect);
	virtual void request_attention();
	virtual void close();

	window();
	~window();
private:
	window_id m_window_id_ = display_manager::INVALID_WINDOW_ID;

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

