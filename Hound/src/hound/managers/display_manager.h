#pragma once
#include "hound/core/base.h"
#include "hound/core/os/os.h"
#include "hound/core/bit.h"
#include "hound/core/object/object.h"
#include "hound/math/math.h"

class window;
class input_event_with_modifier;
class window_event;

class display_manager : public object
{
protected:
	static display_manager* s_instance_;

public:
	typedef int window_id;
	typedef int monitor_id;
	
	enum
	{
		MAIN_WINDOW_ID = 0,
		INVALID_WINDOW_ID = -1
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
		MAIN_MONITOR_ID = 0,
		INVALID_MONITOR_ID = -1
	};
	
	struct window_properties
	{
		rect_i rect;
		vec2_i min_size;
		vec2_i max_size;
		vec2_i aspect;

		int flags;
		window_mode mode;

		std::string title;
		monitor_id monitor_id = MAIN_MONITOR_ID;
	};

	struct window_data
	{
		window_id id;
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

		bool is_centered;
		bool is_resizable;
		bool is_visible;
		bool is_focused;
		bool is_maximized;
		bool is_minimized;
		bool is_always_on_top;
		window_border_style border_style;

		window* window_object;
		monitor_id monitor = MAIN_MONITOR_ID;
	};
	
	static display_manager* get_instance() { return s_instance_; }

	virtual void initialize(const window_properties& main_window_properties, graphics_context* graphics_context);
	virtual void redraw_windows() = 0;

	virtual window_id get_main_window() = 0;
	window_id request_sub_window(window_id parent_id, const window_properties& properties);
	
	void show_window(window_id window);
	void hide_window(window_id window);
	void maximize_window(window_id window);
	void minimize_window(window_id window);
	void resize_window(window_id window, const rect_i& rect);
	void set_window_min_size(window_id window, const vec2_i& min_size);
	void set_window_max_size(window_id window, const vec2_i& max_size);
	void window_set_border_style(window_id window, window_border_style style);
	void window_grab_focus(window_id window);
	void window_request_attention(window_id window);
	
	virtual bool destroy_sub_window(window_id id) = 0;

	virtual void attach_window_object(window*, window_id id = MAIN_WINDOW_ID) = 0;
	virtual window* get_attached_window_object(window_id id = MAIN_WINDOW_ID) = 0;

	virtual void process_all_window_events() = 0;

	typedef void (*input_event_callback)(const input_event_with_modifier& e);
	typedef void (*window_event_callback)(const window_event& e);
	
	static window_properties get_default_properties();

	display_manager();
	virtual ~display_manager();
	
private:
	window_id m_window_id_counter_ = MAIN_WINDOW_ID;
	std::unordered_map<window_id, window_data> m_window_data_;

	const window_data& create_window_data(const window_properties& properties, window* window_object);
	
protected:
	graphics_context* m_graphics_context_;

	const window_data& get_window_data(window_id window);

	void set_main_window(window* window, window_id id);
	void pass_key_event(window* window, uint32_t window_id, key_code key, key_action action, int modifiers);
	void pass_mouse_position_event(window* window, uint32_t window_id, double pos_x, double pos_y);
	void pass_mouse_button_event(window* window, uint32_t window_id, key_code button, key_action action, int modifiers);

	virtual void on_init() = 0;
	
	virtual void create_native_window(const window_data& window_data) = 0;

	virtual void redraw_all_native_windows() = 0;
	virtual void redraw_native_window(window_id id) = 0;

	virtual void poll_native_window_events() = 0;
	
	virtual void set_native_window_title(window_id window, const std::string& title) = 0;
	virtual void set_native_window_rect(window_id window, const rect_i& rect) = 0;
	virtual void set_native_window_min_size(window_id window, const vec2_i& min_size) = 0;
	virtual void set_native_window_max_size(window_id window, const vec2_i& max_size) = 0;
	virtual void set_native_window_mode(window_id window, window_mode mode) = 0;
	virtual void set_native_window_visible(window_id window, bool visible) = 0;
	virtual void set_native_window_focused(window_id window, bool focused) = 0;
	virtual void set_native_window_maximized(window_id window, bool maximized) = 0;
	virtual void set_native_window_minimized(window_id window, bool minimized) = 0;
	virtual void set_native_window_is_always_on_top(window_id window, bool is_always_on_top) = 0;
	virtual void set_native_window_border_style(window_id window, window_border_style style) = 0;
};

