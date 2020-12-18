#pragma once
#include "hound/core/base.h"
#include "hound/core/object/object.h"
#include "hound/core/rendering/renderer_cache.h"

class render_target : public object
{
public:
	HND_PROPERTY_READ_ONLY(size, vec2_i, renderer_cache::render_target_cache()->get_render_target_size(get_object_id()))
	
	virtual void begin_frame() = 0;
	virtual void end_frame() = 0;
};

