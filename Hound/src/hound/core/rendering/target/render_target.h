#pragma once
#include "hound/core/base.h"
#include "hound/core/object/object.h"

HND_OBJECT_BASE_CLASS_DECL(render_target)
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

