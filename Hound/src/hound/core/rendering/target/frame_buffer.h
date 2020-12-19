#pragma once
#include "hound/core/object/object.h"
#include "hound/core/math/math.h"

HND_OBJECT_CLASS_DECL(frame_buffer, frame_buffer_cache_module)
{
	HND_OBJECT_CLASS_FUNC_DECL_1(frame_buffer, frame_buffer_cache_module, const vec2_i&)
	
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

