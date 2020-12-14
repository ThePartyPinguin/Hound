#pragma once
#include "hound/drivers/graphics_context.h"

class open_gl_context : public graphics_context
{
public:
	display_manager::window_id request_window() override;
	
	open_gl_context();
	virtual ~open_gl_context();
	
private:
	bool s_glad_initialized;
	std::string test_string;
};