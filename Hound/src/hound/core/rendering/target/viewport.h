#pragma once
#include "frame_buffer.h"
#include "render_target.h"
#include "hound/core/event/event.h"
#include "hound/core/event/window_event.h"
#include "hound/core/rendering/renderer_cache/cache_object_functions.h"

class window;

CACHED_DERIVED_OBJECT(viewport, render_target, render_target_cache_module), public event_handler<window_frame_buffer_resize_event>
{
	CACHED_OBJECT_DECL(viewport, render_target_cache_module)
	CACHED_OBJECT_CREATE_FUNC_DECL_P1(viewport, const rect_i&, size)
	
public:
	HND_PROPERTY_PTR_READ_ONLY(owner_window, window, m_owner_window_)
	HND_PROPERTY_PTR_READ_ONLY(frame_buffer, frame_buffer, m_frame_buffer_)
	
	void set_owner_window(window* window);


	viewport();
	virtual ~viewport() = default;
	
protected:
	bool m_has_owner_window_;
	window* m_owner_window_;
	
	void begin_frame() override;
	void end_frame() override;
	bool should_render() override;
	
	void on_event(const window_frame_buffer_resize_event& e) override;
	
private:
	void on_set_rect(const rect_i& rect) override;
};
