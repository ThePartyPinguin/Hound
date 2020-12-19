#include "hound/hnd_pch.h"
#include "open_gl_renderer.h"

#include "GLFW/glfw3.h"
#include "hound/core/rendering/renderer.h"
#include "hound/core/rendering/target/render_target.h"
#include "hound/platform/open_gl/object/shader/open_gl_shader.h"
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
	
	render_target->begin_frame();

	HND_GL_CALL(glEnable, GL_DEPTH_TEST);
	HND_GL_CALL(glClearColor, clear_color.get_x(), clear_color.get_y(), clear_color.get_z(), clear_color.get_w());
	HND_GL_CALL(glClear, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void open_gl_renderer::end_frame(render_target* render_target)
{
	render_target->end_frame();
}

void open_gl_renderer::render_indexed(shader_id shader, mesh_id mesh)
{
	open_gl_shader* shader_instance = dynamic_cast<open_gl_shader*>(open_gl_renderer_cache::gl_shader_cache()->get_shader_object(shader));

	open_gl_mesh_cache_module* mesh_cache = open_gl_renderer_cache::gl_mesh_cache();

	open_gl_mesh_cache_module::gl_mesh_object mesh_object = mesh_cache->get_gl_mesh_object(mesh);

	const open_gl_mesh_cache_module::gl_mesh_object& data = mesh_cache->get_gl_mesh_object(mesh);
	
	const gl_object_id vertex_array_object = data.gl_vertex_array_id;

	shader_instance->use();
		
	if(shader_instance->get_name() == "FlatShader")
	{
		shader_instance->set_uniform_float("u_Time", glfwGetTime());
	}

	HND_GL_CALL(glBindVertexArray, vertex_array_object);
	HND_GL_CALL(glDrawElements, GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	HND_GL_CALL(glBindVertexArray, 0);
}

open_gl_renderer::open_gl_renderer()
{
	s_instance_ = this;
}

open_gl_renderer::~open_gl_renderer()
{
}
