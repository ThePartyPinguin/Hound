#pragma once
#include "hound/core/os/os.h"
#include "hound/core/bit.h"
#include "hound/core/object/object.h"
#include "hound/core/math/math.h"
#include "hound/core/rendering/renderer_resource_id.h"

class viewport;
class window;
class input_event_with_modifier;
class window_event;

#define HND_DPM_DEFAULT_WINDOW_DATA 


class display_manager : public object
{
protected:
	friend class os;
	friend class window;
	static display_manager* s_instance_;

public:	
	enum
	{
		MAIN_WINDOW_ID = 1,
		INVALID_WINDOW_ID = 0
	};

	enum window_mode
	{
		windowed = BIT(0),
		minimized = BIT(1),
		maximized = BIT(2),
		full_screen = BIT(3),
		full_screen_windowed = BIT(4)
	};

	enum window_flags
	{
		window_flag_resizable = BIT(1),
		window_flag_border_less = BIT(2),
		window_flag_always_on_top = BIT(3),
		window_flag_focused = BIT(4),
		window_flag_centered = BIT(5)
	};

	enum window_border_style
	{
		border_less,
		bordered
	};

	enum
	{
		MAIN_MONITOR_ID = 1,
		INVALID_MONITOR_ID = 0
	};
	
	struct window_properties
	{
		std::string title;
		rect_i rect;
		vec2_i min_size;
		vec2_i max_size;
		
		window_mode mode;
		bool is_resizable;
		bool is_visible;
		bool is_focused;
		bool is_maximized;
		bool is_minimized;
		bool is_always_on_top;
		window_border_style border_style;
		
		monitor_id monitor_id = MAIN_MONITOR_ID;
	};

	struct window_data
	{
		window_id id = INVALID_WINDOW_ID;
		window_id parent_id = INVALID_WINDOW_ID;
		std::set<window_id> children;

		std::string title;
		rect_i rect;
		rect_i frame_buffer_rect;

		vec2_f content_scale;
		vec2_i min_size;
		vec2_i max_size;
		vec2_i aspect;
		
		window_mode mode;
		bool should_close;
		bool is_resizable;
		bool is_visible;
		bool is_focused;
		bool is_maximized;
		bool is_minimized;
		bool is_always_on_top;
		window_border_style border_style;

		render_target_id viewport;
		
		window* window_object;
		monitor_id monitor_id = MAIN_MONITOR_ID;
	};
	
	static display_manager* get_instance() { return s_instance_; }

	virtual void redraw_windows() = 0;
	virtual void redraw_window(window_id id) = 0;

	virtual const window_data& get_main_window() = 0;
	virtual const window_data& request_sub_window(const std::string& title, const vec2_i& size, window_id parent_id = MAIN_WINDOW_ID) = 0;
	virtual bool destroy_sub_window(window_id id) = 0;
	
	virtual void process_window_events() = 0;

	virtual void window_set_title(window_id window, const std::string& title) = 0;
	virtual void window_set_rect(window_id window, const rect_i& rect) = 0;
	virtual void window_set_frame_buffer_rect(window_id window, const rect_i& rect) = 0;
	virtual void window_set_min_size(window_id window, const vec2_i& min_size) = 0;
	virtual void window_set_max_size(window_id window, const vec2_i& max_size) = 0;
	virtual void window_set_mode(window_id window, window_mode mode) = 0;
	virtual void window_set_visible(window_id window, bool visible) = 0;
	virtual void window_set_focused(window_id window, bool focused) = 0;
	virtual void window_set_resizable(window_id window, bool is_resizable) = 0;
	virtual void window_set_content_scale(window_id window, const vec2_f& scale) = 0;
	virtual void window_set_aspect(window_id window, const vec2_i& aspect) = 0;
	virtual void window_set_maximized(window_id window, bool maximized) = 0;
	virtual void window_set_minimized(window_id window, bool minimized) = 0;
	virtual void window_set_is_always_on_top(window_id window, bool is_always_on_top) = 0;
	virtual void window_set_border_style(window_id window, window_border_style style) = 0;
	virtual void window_request_attention(window_id window) = 0;

	virtual const window_data& get_window_data(window_id window) = 0;
	
	display_manager();
	virtual ~display_manager();	
private:
	window_id m_window_id_counter_ = MAIN_WINDOW_ID;
	
protected:	
	window* create_window_object(window_id window);
	
	void set_main_window(window* window, window_id id);
	void pass_key_event(window* window, uint32_t window_id, key_code key, key_action action, int modifiers);
	void pass_mouse_position_event(window* window, uint32_t window_id, double pos_x, double pos_y);
	void pass_mouse_button_event(window* window, uint32_t window_id, key_code button, key_action action, int modifiers);

	void set_window_frame_buffer(window_id window_id, const rect_i& rect);
	void set_window_aspect(window_id window_id, const vec2_i& aspect);
	void set_window_content_scale(window_id window_id, const vec2_f& scale);	
};

void initialize_display_manager();

