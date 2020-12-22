#pragma once
#include "hound/core/rendering/renderer_cache/cache_object_functions.h"

class frame_buffer;
CACHED_OBJECT(render_target, render_target_cache_module)
{
public:

	HND_PROPERTY_PTR_READ_ONLY(frame_buffer, frame_buffer, m_frame_buffer_)
	
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

	render_target();
	virtual ~render_target();
	
	type m_target_type_ = type::INVALID;
	frame_buffer* m_frame_buffer_;
};

