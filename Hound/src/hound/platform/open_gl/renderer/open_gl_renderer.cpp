#include "hound/hnd_pch.h"
#include "open_gl_renderer.h"


#include "GLFW/glfw3.h"
#include "hound/platform/open_gl/object/shader/open_gl_shader.h"
#include "hound/core/rendering/renderer.h"
#include "hound/platform/open_gl/renderer/renderer_cache/open_gl_renderer_cache.h"
#include "hound/core/object/object_database.h"
#include "renderer_cache/module/open_gl_mesh_cache_module.h"

renderer::type_api open_gl_renderer::get_api_type()
{
	return type_api::OPEN_GL;
}

void open_gl_renderer::begin_frame()
{
	const vec4_f& clear_color = get_clear_color();
	glClearColor(clear_color.get_x(), clear_color.get_y(), clear_color.get_z(), clear_color.get_w());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void open_gl_renderer::render_indexed(shader_cache_module::shader_id shader, mesh_cache_module::mesh_id mesh)
{
	open_gl_shader* shader_instance = object_database::get_instance()->get_object_instance<open_gl_shader>(shader);

	open_gl_mesh_cache_module* mesh_cache = renderer_cache::get_module<open_gl_mesh_cache_module>();
	
	const gl_object_id vertex_array_object = mesh_cache->get_gl_mesh_object(mesh).gl_vertex_array_id;

	// glUseProgram(shader_instance->get_shader_program_id());
	shader_instance->use();
	shader_instance->set_uniform_float("u_Time", glfwGetTime());

	glBindVertexArray(vertex_array_object);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void open_gl_renderer::end_frame()
{
}

open_gl_renderer::open_gl_renderer()
{
	s_instance_ = this;
}

open_gl_renderer::~open_gl_renderer()
{
}
