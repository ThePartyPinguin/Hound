#pragma once
#include "hound/core/object/object.h"
#include "hound/core/math/math.h"

class frame_buffer : public object
{
public:
	HND_PROPERTY_CALLBACK(size, vec2_i, m_size_, on_set_size)

	virtual void create() = 0;
	virtual void bind() = 0;
	virtual void un_bind() = 0;
	
protected:
	virtual void on_set_size(const vec2_i& size) = 0;
	
	frame_buffer() = default;
	virtual ~frame_buffer() = default;
	
private:
	vec2_i m_size_;
};

