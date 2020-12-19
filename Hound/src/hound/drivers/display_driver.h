#pragma once
#include "hound/core/rendering/renderer_resource_id.h"
#include "hound/core/window/window_enum.h"

class viewport;
class monitor;
class window;

class display_driver : public object
{
	friend class window;
public:	
	enum
	{
		MAIN_WINDOW_ID = 0,
		INVALID_WINDOW_ID = -1
	};
	
	enum
	{
		MAIN_MONITOR_ID = 0,
		INVALID_MONITOR_ID = -1
	};

	static display_driver* get_instance() { return s_instance_; };
	
	virtual void init() = 0;
	virtual window_id create_window(const char* title, const vec2_i& size, window_id parent = MAIN_WINDOW_ID, monitor_id monitor_id = MAIN_MONITOR_ID) = 0;
	virtual void destroy_window(window_id window) = 0;
	virtual window* get_window_handle(window_id window) = 0;
	virtual void* get_native_window_handle(window_id window) = 0;
	virtual void* get_native_proc_address() = 0;
	virtual void redraw_window(window_id window) = 0;
	virtual void process_window_events() = 0;

	virtual monitor* get_monitor_handle(monitor_id monitor) = 0;
	virtual bool is_window_on_monitor(window_id window, monitor_id monitor) = 0;
	virtual monitor_id get_native_monitor(window_id window) = 0;

	virtual void set_window_viewport(window_id window, viewport* viewport) = 0;
	
protected:
	static display_driver* s_instance_;
	
	display_driver() = default;
	virtual ~display_driver() = default;
		
	struct window_data
	{
		window_id id = INVALID_WINDOW_ID;
		window_id parent = INVALID_WINDOW_ID;
		std::set<window_id> children;
		window* object_handle = nullptr;
		monitor_id monitor;
	};

	virtual void native_request_attention(window_id window) = 0;
	virtual void set_native_title(window_id window, const char* title) = 0;
	virtual void set_native_rect(window_id window, const rect_i& rect) = 0;
	virtual void set_native_min_size(window_id window, const vec2_i& size) = 0;
	virtual void set_native_max_size(window_id window, const vec2_i& size) = 0;
	virtual void set_native_aspect(window_id window, const vec2_i& aspect) = 0;
	virtual void set_native_mode(window_id window, window_mode aspect) = 0;
	virtual void set_native_should_close(window_id window, bool should_close) = 0;
	virtual void set_native_resizable(window_id window, bool is_resizable) = 0;
	virtual void set_native_visible(window_id window, bool is_visible) = 0;
	virtual void set_native_focus(window_id window, bool is_focused) = 0;
	virtual void set_native_maximized(window_id window, bool is_maximized) = 0;
	virtual void set_native_minimized(window_id window, bool is_minimized) = 0;
	virtual void set_native_always_on_top(window_id window, bool is_always_on_top) = 0;
	virtual void set_native_border_style(window_id window, window_border_style style) = 0;

	struct monitor_data
	{
		monitor_id id = INVALID_MONITOR_ID;
		std::set<window_id> containing_windows;
		monitor* object_handle;
	};

	virtual void add_window_to_monitor(window_id window, monitor_id monitor) = 0;
	virtual void remove_window_from_monitor(window_id window, monitor_id monitor) = 0;
};
