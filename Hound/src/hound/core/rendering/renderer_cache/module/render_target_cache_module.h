#pragma once
#include "renderer_cache_module.h"

class render_target;
class viewport;

class render_target_cache_module : public renderer_cache_module
{
public:
	enum target_type
	{
		VIEWPORT,
		TEXTURE
	};
	
	struct render_target_data
	{
		render_target_id id;
		target_type type;
		render_target* target_object;
		frame_buffer_id frame_buffer;
		window_id parent_window;
	};

	virtual void render_target_set_frame_buffer_size(render_target_id render_target, vec2_i size) = 0;

	virtual render_target* get_render_target(render_target_id render_target) = 0;
	virtual viewport* get_viewport(render_target_id render_target) = 0;
	virtual void set_viewport_parent_window(render_target_id viewport, window_id window) = 0;
	virtual window_id get_viewport_parent_window(render_target_id viewport) = 0;
	virtual bool viewport_has_parent_window(render_target_id viewport) = 0;
	virtual void unset_viewport_parent_window(render_target_id viewport) = 0;
	
	virtual render_target_id create_render_target(target_type type, const vec2_i& size) = 0;
	virtual frame_buffer_id get_target_frame_buffer(render_target_id target) = 0;
	virtual const vec2_i& get_render_target_size(render_target_id render_target) = 0;

	virtual std::set<render_target_id> get_render_targets() = 0;
	
protected:
	render_target_cache_module() = default;
	virtual ~render_target_cache_module() = default;
};

#define RTC render_target_cache_module
