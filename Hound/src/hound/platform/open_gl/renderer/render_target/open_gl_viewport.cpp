#include "hound/hnd_pch.h"
#include "open_gl_viewport.h"


#include "hound/core/rendering/renderer.h"
#include "hound/platform/open_gl/context/open_gl_context.h"
#include "hound/platform/open_gl/renderer/renderer_cache/open_gl_renderer_cache.h"
#include "hound/platform/open_gl/renderer/renderer_cache/module/open_gl_frame_buffer_cache_module.h"
#include "hound/platform/open_gl/renderer/renderer_cache/module/open_gl_render_target_cache_module.h"

void open_gl_viewport::begin_frame()
{
	const frame_buffer_id frame_buffer = open_gl_renderer_cache::gl_render_target_cache()->get_target_frame_buffer(get_object_id());
	open_gl_renderer_cache::gl_frame_buffer_cache()->bind_gl_frame_buffer(frame_buffer);
}

void open_gl_viewport::end_frame()
{
	const frame_buffer_id frame_buffer = open_gl_renderer_cache::gl_render_target_cache()->get_target_frame_buffer(get_object_id());
	open_gl_renderer_cache::gl_frame_buffer_cache()->un_bind_gl_frame_buffer(frame_buffer);
	
	const auto& data = open_gl_renderer_cache::gl_frame_buffer_cache()->get_gl_frame_buffer_data(frame_buffer);
	
	open_gl_renderer_cache::gl_texture_cache()->bind_texture(data.color_buffer_texture_id);
	
	const shader_id screen_shader = open_gl_renderer_cache::gl_shader_cache()->get_standard_screen_shader();
	const mesh_id screen_mesh = open_gl_renderer_cache::gl_frame_buffer_cache()->get_frame_buffer_quad();
	renderer::get_instance()->render_indexed(screen_shader, screen_mesh);
}
