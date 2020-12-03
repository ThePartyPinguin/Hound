#pragma once
#include "hound/core/object/object.h"
#include "hound/managers/display_manager.h"

class graphics_context : public object
{
public:
	static void initialize();
	
	static graphics_context* get_instance();

	virtual display_manager::window_id request_window() = 0;
	
protected:
	static graphics_context* s_instance_;
	
	graphics_context();
	virtual ~graphics_context();
};

void initialize_graphics_context();
