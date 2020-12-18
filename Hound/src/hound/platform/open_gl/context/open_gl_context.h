#pragma once
#include "GLFW/glfw3.h"
#include "hound/core/rendering/renderer_resource_id.h"
#include "hound/drivers/graphics_context.h"

class open_gl_context : public graphics_context
{
public:
	window_id request_window() override;

	void set_window_context_active(GLFWwindow* window);
	
	open_gl_context();
	virtual ~open_gl_context();
	
private:
	bool s_glad_initialized;
	std::string test_string;
};