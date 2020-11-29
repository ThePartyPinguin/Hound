#pragma once
#include "hound/core/object/object.h"

class graphics_context : public object
{
public:
	static graphics_context* get_instance();

	virtual void initialize() = 0;
	
	graphics_context();
private:
	static graphics_context* s_instance_;
};

