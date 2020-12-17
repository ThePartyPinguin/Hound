#pragma once
#include "hound/core/rendering/target/render_target.h"

class window;

class viewport : public render_target
{
public:
	HND_PROPERTY_READ_ONLY(parent_window, window_id, renderer_cache::render_target_cache()->get_viewport_parent_window(get_object_id()))
	HND_PROPERTY_READ_ONLY(has_parent_window, bool, renderer_cache::render_target_cache()->viewport_has_parent_window(get_object_id()))
	
	viewport() = default;
	virtual ~viewport() = default;
	
protected:
	
	void begin_frame() override;
	void end_frame() override;
};
