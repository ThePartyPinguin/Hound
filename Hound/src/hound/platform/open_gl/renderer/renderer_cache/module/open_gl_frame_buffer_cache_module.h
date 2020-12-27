#pragma once
#include "hound/core/rendering/renderer_cache/module/frame_buffer_cache_module.h"
#include "hound/core/rendering/renderer_cache/module/texture_cache_module.h"
#include "hound/platform/open_gl/open_gl_core.h"

class open_gl_frame_buffer_cache_module : public frame_buffer_cache_module
{
public:

	struct gl_frame_buffer_data : internal_frame_buffer_data
	{
		gl_object_id gl_frame_buffer_object_id;
		gl_object_id gl_depth_stencil_object_id;
		texture_id color_buffer_texture_id;
		vec2_i size;
	};

	void on_create_instance(frame_buffer* instance, const vec2_i& size) override;

	void bind_frame_buffer(frame_buffer_id frame_buffer) override;
	void un_bind_frame_buffer(frame_buffer_id frame_buffer) override;

	void frame_buffer_set_size(frame_buffer_id frame_buffer, const vec2_i& size) override;
	const vec2_i& frame_buffer_get_size(frame_buffer_id frame_buffer) override;

	const gl_frame_buffer_data& get_gl_frame_buffer_data(frame_buffer_id frame_buffer);

	mesh_id get_frame_buffer_quad();

	void init() override;
	
	open_gl_frame_buffer_cache_module();
	virtual ~open_gl_frame_buffer_cache_module();

private:

	mesh_id m_frame_buffer_mesh_id_;
	
	std::unordered_map<frame_buffer_id, gl_frame_buffer_data> m_gl_frame_buffer_map_;

	const char* get_gl_frame_buffer_status_msg(GLint status);
};

#define HND_GL_FBC open_gl_frame_buffer_cache_module
