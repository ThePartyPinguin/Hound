#include "hound/hnd_pch.h"
#include "open_gl_camera_cache_module.h"
#include "hound/core/rendering/camera/camera.h"
#include "hound/platform/open_gl/open_gl_constants.h"

RENDER_CACHE_CLASS_IMPL(camera_cache_module, camera)
RENDER_CACHE_CREATE_FUNC_IMPL_NAMED_P2(camera, camera_cache_module, as_orthographic, render_target*, target, const orthographic_projection_settings&, settings)
RENDER_CACHE_CREATE_FUNC_IMPL_NAMED_P2(camera, camera_cache_module, as_perspective, render_target*, target, const perspective_projection_settings&, settings)


void open_gl_camera_cache_module::on_create_instance_as_orthographic(camera* instance, render_target* target, const orthographic_projection_settings& settings)
{
	
	gl_camera_data& gl_data = m_camera_data_map_[instance->get_object_id()];
	gl_data.id = instance->get_object_id();
	gl_data.handle = instance;
	gl_data.render_target = instance->get_render_target();
	gl_data.matrix_data = { instance->get_view_matrix(), instance->get_projection_matrix() };

	//Create uniform buffer and reserve buffer size
	HND_GL_CALL(glGenBuffers, 1, &gl_data.gl_matrix_ubo_id);
	HND_GL_CALL(glBindBuffer, GL_UNIFORM_BUFFER, gl_data.gl_matrix_ubo_id);
	// glBufferData(GL_UNIFORM_BUFFER, gl_camera_matrix_data::gl_reserve_size, NULL, GL_DYNAMIC_DRAW);
	HND_GL_CALL(glBufferData, GL_UNIFORM_BUFFER, gl_camera_matrix_data::gl_reserve_size, nullptr, GL_DYNAMIC_DRAW);
	HND_GL_CALL(glBindBuffer, GL_UNIFORM_BUFFER, 0);

	set_gl_ubo_data(gl_data.id, 0, gl_camera_matrix_data::gl_reserve_size, &gl_data.matrix_data);	
}

void open_gl_camera_cache_module::on_create_instance_as_perspective(camera* instance, render_target* target, const perspective_projection_settings& settings)
{
}

void open_gl_camera_cache_module::update_camera_view_matrix(resource_id camera, const mat4_f& view_matrix)
{
	gl_camera_data& gl_data = get_camera_data(camera);
	gl_data.matrix_data.view_matrix = view_matrix;
	set_gl_ubo_data(camera, gl_camera_matrix_data::view_matrix_offset, gl_camera_matrix_data::view_matrix_size, &gl_data.matrix_data.view_matrix);
}

void open_gl_camera_cache_module::update_camera_projection_matrix(resource_id camera, const mat4_f& projection_matrix)
{
	gl_camera_data& gl_data = get_camera_data(camera);
	gl_data.matrix_data.projection_matrix = projection_matrix;
	set_gl_ubo_data(camera, gl_camera_matrix_data::projection_matrix_offset, gl_camera_matrix_data::projection_matrix_size, &gl_data.matrix_data.projection_matrix);
}

void open_gl_camera_cache_module::camera_render_begin(resource_id camera)
{
	const gl_camera_data& gl_data = get_camera_data(camera);
	HND_GL_CALL(glBindBufferBase, GL_UNIFORM_BUFFER, gl_binding_constant::camera_matrices, gl_data.gl_matrix_ubo_id);
}

void open_gl_camera_cache_module::camera_render_end(resource_id camera)
{
	HND_GL_CALL(glBindBufferBase, GL_UNIFORM_BUFFER, gl_binding_constant::camera_matrices, 0);
}

void open_gl_camera_cache_module::init()
{
	
}

open_gl_camera_cache_module::open_gl_camera_cache_module()
{
	s_instance_ = this;
}

open_gl_camera_cache_module::~open_gl_camera_cache_module()
{
}

open_gl_camera_cache_module::gl_camera_data& open_gl_camera_cache_module::get_camera_data(resource_id camera)
{
	if(!m_camera_data_map_.count(camera))
	{
		HND_CORE_LOG_WARN("Camera does not exist!");
		return m_camera_data_map_[resource_id::null()];
	}

	return m_camera_data_map_[camera];
}

void open_gl_camera_cache_module::set_gl_ubo_data(resource_id camera, size_t offset, size_t size, void* data)
{
	const gl_camera_data& gl_data = get_camera_data(camera);
	HND_GL_CALL(glBindBuffer, GL_UNIFORM_BUFFER, gl_data.gl_matrix_ubo_id);
	HND_GL_CALL(glBufferSubData, GL_UNIFORM_BUFFER, offset, size, data);
	HND_GL_CALL(glBindBuffer, GL_UNIFORM_BUFFER, 0);
}

