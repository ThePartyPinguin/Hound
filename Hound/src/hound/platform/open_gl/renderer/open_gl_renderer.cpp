#include "hound/hnd_pch.h"
#include "open_gl_renderer.h"

#include "GLFW/glfw3.h"
#include "hound/core/object/shader/shader.h"
#include "hound/core/rendering/renderer.h"
#include "hound/core/rendering/camera/camera.h"
#include "hound/core/rendering/target/render_target.h"
#include "hound/core/rendering/target/frame_buffer.h"
#include "hound/platform/open_gl/renderer/renderer_cache/module/open_gl_frame_buffer_cache_module.h"
#include "hound/platform/open_gl/renderer/renderer_cache/module/open_gl_mesh_cache_module.h"
#include "hound/platform/open_gl/renderer/renderer_cache/open_gl_renderer_cache.h"

renderer::type_api open_gl_renderer::get_api_type()
{
	return type_api::OPEN_GL;
}

void open_gl_renderer::begin_frame(render_target* render_target)
{
	const vec4_f& clear_color = get_clear_color();

	m_current_render_target_ = render_target;
	m_current_render_target_->begin_frame();
	const vec2_i& size = m_current_render_target_->get_frame_buffer()->get_size();

	HND_GL_CALL(glViewport, 0, 0, size.get_x(), size.get_y());
	
	HND_GL_CALL(glEnable, GL_DEPTH_TEST);
	HND_GL_CALL(glClearColor, clear_color.get_x(), clear_color.get_y(), clear_color.get_z(), clear_color.get_w());
	HND_GL_CALL(glClear, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void open_gl_renderer::end_frame(render_target* render_target)
{
	m_current_render_target_->end_frame();

	HND_GL_CALL(glDisable, GL_DEPTH_TEST);
	HND_GL_CALL(glClearColor, 1.0f, 1.0f, 1.0f, 1.0f);
	HND_GL_CALL(glClear, GL_COLOR_BUFFER_BIT);
	
	draw_frame_buffer(m_current_render_target_);
}

void open_gl_renderer::render_indexed(shader_id shader_id, mesh_id mesh)
{
	shader* shader_instance = open_gl_renderer_cache::gl_shader_cache()->get_shader_object(shader_id);

	open_gl_mesh_cache_module* mesh_cache = open_gl_renderer_cache::gl_mesh_cache();

	open_gl_mesh_cache_module::gl_mesh_object mesh_object = mesh_cache->get_gl_mesh_object(mesh);

	const open_gl_mesh_cache_module::gl_mesh_object& data = mesh_cache->get_gl_mesh_object(mesh);
	
	const gl_object_id vertex_array_object = data.gl_vertex_array_id;

	shader_instance->use();
		
	if(shader_instance->get_name() == "FlatShader")
	{
		float time = glfwGetTime();
		shader_instance->set_uniform_float("u_Time", time);
		shader_instance->set_uniform_mat4_f("model", mat4_f::create_identity() * mat4_f::from_translation({ 0.0f,0.0f,1.0f }));
	}

	HND_GL_CALL(glBindVertexArray, vertex_array_object);
	HND_GL_CALL(glDrawElements, GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	HND_GL_CALL(glBindVertexArray, 0);
}

void open_gl_renderer::draw_frame_buffer(render_target* render_target)
{
	frame_buffer* fb = render_target->get_frame_buffer();
	const open_gl_frame_buffer_cache_module::gl_frame_buffer_data& fb_data = open_gl_renderer_cache::gl_frame_buffer_cache()->get_gl_frame_buffer_data(fb->get_object_id());

	const open_gl_texture_cache_module::gl_texture_data& fb_tex_data = open_gl_renderer_cache::gl_texture_cache()->get_gl_texture_data(fb_data.color_buffer_texture_id);

	HND_GL_CALL(glBindTexture, GL_TEXTURE_2D, fb_tex_data.gl_texture_object_id);
	
	shader_id shader = open_gl_renderer_cache::gl_shader_cache()->get_standard_screen_shader();
	mesh_id fb_mesh = open_gl_renderer_cache::gl_frame_buffer_cache()->get_frame_buffer_quad();

	// HND_GL_CALL(glPolygonMode, GL_FRONT_AND_BACK, GL_LINE);
	
	
	render_indexed(shader, fb_mesh);
}

open_gl_renderer::open_gl_renderer()
{
	s_instance_ = this;
}

open_gl_renderer::~open_gl_renderer()
{
}
