#pragma once
#include "hound/drivers/graphics_context.h"

class open_gl_context : public graphics_context
{
public:
	void initialize() override;
	
	open_gl_context();
private:
	bool s_glad_initialized;
	std::string test_string;
};