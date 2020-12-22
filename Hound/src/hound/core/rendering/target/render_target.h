#pragma once
#include "hound/core/rendering/renderer_cache/cache_object_functions.h"

CACHED_OBJECT(render_target, render_target_cache_module)
{
public:
	enum type
	{
		INVALID,
		VIEWPORT,
		RENDER_TEXTURE
	};
	
	HND_PROPERTY_READ_ONLY(type, type, m_target_type_)
	
	virtual void begin_frame() = 0;
	virtual void end_frame() = 0;

protected:
	type m_target_type_ = type::INVALID;
};

