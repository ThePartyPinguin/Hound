#pragma once
#include "hound/core/rendering/renderer_cache/cache_object_functions.h"

class camera;
class frame_buffer;
CACHED_OBJECT(render_target, render_target_cache_module)
{
public:
	HND_PROPERTY_PTR_READ_ONLY(frame_buffer, frame_buffer, m_frame_buffer_)
	HND_PROPERTY_PTR_READ_ONLY(camera, camera, m_camera_)
	HND_PROPERTY_CALLBACK(rect, rect_i, m_rect_, on_set_rect)
	
	enum type
	{
		INVALID,
		VIEWPORT,
		RENDER_TEXTURE
	};
	
	HND_PROPERTY_READ_ONLY(type, type, m_target_type_)
	
	virtual void begin_frame() = 0;
	virtual void end_frame() = 0;
	virtual bool should_render() = 0;

	void set_camera(camera* camera);
	
protected:
	render_target();
	virtual ~render_target();

	rect_i m_rect_;
	type m_target_type_ = INVALID;
	frame_buffer* m_frame_buffer_ = nullptr;
	camera* m_camera_ = nullptr;

	virtual void on_set_rect(const rect_i& rect) = 0;
};

