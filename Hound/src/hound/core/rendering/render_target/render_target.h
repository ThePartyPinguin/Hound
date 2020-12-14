#pragma once
#include "hound/core/base.h"
#include "hound/core/property.h"
#include "hound/core/object/object.h"


class render_target : public object
{
public:
	HND_PROPERTY_READ_ONLY(frame_buffer, object_id, m_frame_buffer_)

	friend class renderer;
	friend class display_manager;
	
	HND_PROPERTY(size, vec2_i, m_size_)
	
	render_target();
	virtual ~render_target();
private:

	void frame_render_start();
	void frame_render_end();
	
	vec2_i m_size_;
	object_id m_frame_buffer_;

protected:
	HND_PROPERTY_WRITE_ONLY(frame_buffer, object_id, m_frame_buffer_)
	virtual void on_frame_render_start();
	virtual void on_frame_render_end();
};

