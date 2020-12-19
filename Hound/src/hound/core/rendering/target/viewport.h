#pragma once
#include "hound/core/event/event.h"
#include "hound/core/event/window_event.h"
#include "hound/core/rendering/target/render_target.h"

class window;

HND_OBJECT_DERIVED_CLASS_DECL(viewport, render_target, viewport_cache_module)
{
	HND_OBJECT_CLASS_FUNC_DECL(viewport, viewport_cache_module)
	
public:
	HND_PROPERTY_READ_ONLY(parent_window, window_id, renderer_cache::render_target_cache()->get_viewport_parent_window(get_object_id()))
	HND_PROPERTY_READ_ONLY(has_parent_window, bool, renderer_cache::render_target_cache()->viewport_has_parent_window(get_object_id()))
	HND_PROPERTY_PTR_READ_ONLY(owner_window, window, m_owner_window_)

	HND_PROPERTY_CALLBACK(size, vec2_i, m_size_, on_set_size)
	
	viewport() = default;
	virtual ~viewport() = default;
		
protected:
	bool m_has_owner_window_;
	window* m_owner_window_;

	vec2_i m_size_;
	
	void begin_frame() override;
	void end_frame() override;

private:
	void on_set_size(const vec2_i& size);
};
