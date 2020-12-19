#pragma once
#include "frame_buffer.h"
#include "hound/core/event/window_event.h"
#include "hound/core/rendering/target/render_target.h"

class window;

HND_OBJECT_DERIVED_CLASS_DECL(viewport, render_target, render_target_cache_module)
{
	friend class window;
	HND_OBJECT_CLASS_FUNC_DECL_1(viewport, render_target_cache_module, const vec2_i&)
	
public:
	HND_PROPERTY_PTR_READ_ONLY(owner_window, window, m_owner_window_)
	HND_PROPERTY_PTR_READ_ONLY(frame_buffer, frame_buffer, m_frame_buffer_)
	HND_PROPERTY_CALLBACK(size, vec2_i, m_size_, on_set_size)
	
	void set_owner_window(window* window);


	viewport();
	virtual ~viewport() = default;
	
protected:
	bool m_has_owner_window_;
	window* m_owner_window_;

	frame_buffer* m_frame_buffer_;
	
	vec2_i m_size_;
	
	void begin_frame() override;
	void end_frame() override;
	
private:
	void on_set_size(const vec2_i& size);
};
