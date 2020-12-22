#pragma once
#include "hound/core/math/math.h"
#include "hound/core/rendering/renderer_cache/renderer_cache_module.h"
#include "hound/core/rendering/renderer_cache/cache_module_functions.h"
#include "hound/core/rendering/renderer_resource_id.h"

RENDER_CACHE_CLASS(frame_buffer_cache_module, frame_buffer)
{
	RENDER_CACHE_CLASS_DECL(frame_buffer_cache_module, frame_buffer)
	RENDER_CACHE_CREATE_FUNC_DECL_P1(frame_buffer, const vec2_i&, size)
	RENDER_CACHE_DEFAULT_DATA_STRUCT_DECL(frame_buffer)
	
public:	
	virtual void bind_frame_buffer(frame_buffer_id frame_buffer) = 0;
	virtual void un_bind_frame_buffer(frame_buffer_id frame_buffer) = 0;
	virtual void frame_buffer_set_size(frame_buffer_id frame_buffer, const vec2_i& size) = 0;
	virtual const vec2_i& frame_buffer_get_size(frame_buffer_id frame_buffer) = 0;

protected:
	frame_buffer_cache_module() = default;
	virtual ~frame_buffer_cache_module() = default;
};

#define FBC frame_buffer_cache_module
