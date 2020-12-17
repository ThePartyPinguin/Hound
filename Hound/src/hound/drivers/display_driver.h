#pragma once
#include "hound/core/rendering/renderer_resource_id.h"

class window;

class display_driver : public object
{
public:
	enum
	{
		MAIN_WINDOW_ID = 1,
		INVALID_WINDOW_ID = 0
	};
	
	enum
	{
		MAIN_MONITOR_ID = 1,
		INVALID_MONITOR_ID = 0
	};

	static display_driver* get_instance();
	
	virtual void init() = 0;
	virtual window_id create_window(const char* title, const vec2_i& size, window_id parent = MAIN_WINDOW_ID) = 0;
	virtual void destroy_window(window_id window) = 0;
	virtual window* get_window_handle(window_id window) = 0;
	virtual void* get_native_window_handle(window_id window) = 0;
	virtual void* get_native_proc_address();
	virtual void redraw_window(window_id window) = 0;
	virtual void process_window_events() = 0;

protected:
	static display_driver* s_instance;
	
	display_driver() = default;
	virtual ~display_driver() = default;
	
	
	struct window_data
	{
		window_id id = INVALID_WINDOW_ID;
		window_id parent = INVALID_WINDOW_ID;
		std::set<window_id> children;
		window* object_handle;
	};
};
