#pragma once
#include "hound/core/base.h"
#include "hound/core/object/object.h"

class render_target : public object
{
public:
	virtual void begin_frame() = 0;
	virtual void end_frame() = 0;
};

