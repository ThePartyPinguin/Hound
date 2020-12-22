#pragma once
#include "hound/core/object/object.h"
#include "hound/core/rendering/renderer_cache/cache_object_functions.h"
#include "hound/core/math/math.h"

CACHED_OBJECT(frame_buffer, frame_buffer_cache_module)
{
	CACHED_OBJECT_DECL(frame_buffer, frame_buffer_cache_module)
	CACHED_OBJECT_CREATE_FUNC_DECL_P1(frame_buffer, const vec2_i&, size)
	
public:
	HND_PROPERTY_CALLBACK(size, vec2_i, m_size_, on_set_size)
	
	void bind();
	void un_bind();
	
	frame_buffer() = default;
	virtual ~frame_buffer() = default;
protected:
	void on_set_size(const vec2_i& size);
	
private:
	vec2_i m_size_;
};

