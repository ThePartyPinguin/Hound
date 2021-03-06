#pragma once
#include "hound/core/base.h"
#include "hound/core/property.h"
#include "hound/math/math.h"

class render_target
{
public:
	HND_PROPERTY(size, vec2_i, m_size_)

	render_target() = default;
	
private:
	vec2_i m_size_;
};

