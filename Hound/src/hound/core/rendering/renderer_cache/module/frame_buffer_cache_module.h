#pragma once
#include "hound/core/math/math.h"
#include "hound/core/rendering/renderer_cache/module/renderer_cache_module.h"

class frame_buffer_cache_module : public renderer_cache_module
{
public:
	typedef object_id frame_buffer_id;
	
	struct frame_buffer_data
	{
		vec2_i size;
	};

	virtual frame_buffer_id frame_buffer_create(const vec2_i& size) = 0;
	virtual void frame_buffer_set_size(frame_buffer_id frame_buffer, const vec2_i& size) = 0;
	virtual const vec2_i& frame_buffer_get_size(frame_buffer_id frame_buffer) = 0;

protected:
	frame_buffer_cache_module() = default;
	virtual ~frame_buffer_cache_module() = default;
};

#define FBC frame_buffer_cache_module
