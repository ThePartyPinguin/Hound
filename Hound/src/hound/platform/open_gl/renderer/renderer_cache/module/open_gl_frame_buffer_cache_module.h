#pragma once
#include "hound/core/rendering/renderer_cache/module/frame_buffer_cache_module.h"
#include "hound/core/rendering/renderer_cache/module/texture_cache_module.h"
#include "hound/platform/open_gl/open_gl_core.h"

class open_gl_frame_buffer_cache_module : public frame_buffer_cache_module
{
public:
	struct gl_frame_buffer_data : frame_buffer_data
	{
		gl_object_id gl_frame_buffer_object_id;
		texture_cache_module::texture_id color_buffer_texture_id;
	};

	frame_buffer_id frame_buffer_create(const vec2_i& size) override;
	void frame_buffer_set_size(frame_buffer_id frame_buffer, const vec2_i& size) override;
	const vec2_i& frame_buffer_get_size(frame_buffer_id frame_buffer) override;

	const gl_frame_buffer_data& get_gl_frame_buffer_data(frame_buffer_id frame_buffer);
	
	open_gl_frame_buffer_cache_module();
	virtual ~open_gl_frame_buffer_cache_module();

private:
	std::unordered_map<frame_buffer_id, gl_frame_buffer_data> m_gl_frame_buffer_map_;
};

#define GL_FBC open_gl_frame_buffer_cache_module
