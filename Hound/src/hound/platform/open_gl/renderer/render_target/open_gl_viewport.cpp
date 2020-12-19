#include "hound/hnd_pch.h"
#include "open_gl_viewport.h"


#include "hound/core/rendering/renderer.h"
#include "hound/platform/open_gl/context/open_gl_context.h"
#include "hound/platform/open_gl/object/shader/open_gl_shader.h"
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

	HND_GL_CALL(glDisable, GL_DEPTH_TEST);
	HND_GL_CALL(glClearColor, 1.0f, 1.0f, 1.0f, 1.0f);
	HND_GL_CALL(glClear, GL_COLOR_BUFFER_BIT);
	
	const auto& data = open_gl_renderer_cache::gl_frame_buffer_cache()->get_gl_frame_buffer_data(frame_buffer);
	
	const shader_id screen_shader = open_gl_renderer_cache::gl_shader_cache()->get_standard_screen_shader();
	const mesh_id screen_mesh = open_gl_renderer_cache::gl_frame_buffer_cache()->get_frame_buffer_quad();

	open_gl_shader* shader_instance = dynamic_cast<open_gl_shader*>(open_gl_renderer_cache::gl_shader_cache()->get_shader_object(screen_shader));

	shader_instance->use();
	HND_GL_CALL(glActiveTexture, GL_TEXTURE0);
	open_gl_renderer_cache::gl_texture_cache()->bind_texture(data.color_buffer_texture_id);
	
	open_gl_mesh_cache_module* mesh_cache = open_gl_renderer_cache::gl_mesh_cache();

	const open_gl_mesh_cache_module::gl_mesh_object& gl_mesh_object = mesh_cache->get_gl_mesh_object(screen_mesh);

	const gl_object_id vertex_array_object = gl_mesh_object.gl_vertex_array_id;

	HND_GL_CALL(glBindVertexArray, vertex_array_object);
	HND_GL_CALL(glDrawElements, GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
