#include "hound/hnd_pch.h"
#include "camera.h"

#include "hound/core/rendering/renderer_cache/module/camera_cache_module.h"

GET_CACHE_FUNC_IMPL(camera, camera_cache_module)
CACHED_OBJECT_CREATE_FUNC_IMPL_NAMED_P2(camera, camera_cache_module, as_orthographic, render_target*, target, const orthographic_projection_settings&, settings)
CACHED_OBJECT_CREATE_FUNC_IMPL_NAMED_P2(camera, camera_cache_module, as_perspective, render_target*, target, const perspective_projection_settings&, settings)

void camera::init_as_perspective(render_target* target, const perspective_projection_settings& settings)
{
	m_render_target_ = target;
	
	const float ar = settings.viewport_size.get_x() / settings.viewport_size.get_y();
	const float z_range = settings.z_near - settings.z_far;
	const float tan_half_fov = math::tan(math::deg_to_rad(settings.fov) * 0.5);

	mat4_f& mat = m_projection_matrix_;

	mat[0][0] = 1.0f / (tan_half_fov * ar);
	mat[0][1] = 0.0f;
	mat[0][2] = 0.0f;
	mat[0][3] = 0.0f;

	mat[1][0] = 0.0f;
	mat[1][1] = 1.0f / tan_half_fov;
	mat[1][2] = 0.0f;
	mat[1][3] = 0.0f;

	mat[2][0] = 0.0f;
	mat[2][1] = 0.0f;
	mat[2][2] = (-settings.z_near - settings.z_far) / z_range;
	mat[2][3] = 2.0f * settings.z_far * settings.z_near / z_range;

	mat[3][0] = 0.0f;
	mat[3][1] = 0.0f;
	mat[3][2] = 1.0f;
	mat[3][3] = 0.0f;

	m_view_matrix_.set_identity();
}

void camera::begin_frame()
{
	get_cache()->camera_render_begin(get_object_id());
}

void camera::end_frame()
{
	get_cache()->camera_render_end(get_object_id());
}

void camera::on_render_target_size_set(const vec2_f& size)
{
	m_projection_matrix_ = create_orthographic_projection_matrix(size, m_orthographic_size_, m_z_far_, m_z_near_);
	get_cache()->update_camera_projection_matrix(get_object_id(), m_projection_matrix_);
}

void camera::on_orthographic_size_set(const float& size)
{
	m_projection_matrix_ = create_orthographic_projection_matrix(m_render_target_size_, size, m_z_far_, m_z_near_);
	get_cache()->update_camera_projection_matrix(get_object_id(), m_projection_matrix_);
}

mat4_f camera::create_orthographic_projection_matrix(const vec2_f& target_size, float orthographic_size, float z_far, float z_near)
{
	mat4_f mat = mat4_f::create_identity();

	const float ar = target_size.get_x() / target_size.get_y();
	
	const float left = -(ar * orthographic_size * 0.5f);
	const float right = ar * orthographic_size * 0.5f;
	const float bottom = -(orthographic_size * 0.5f);
	const float top = orthographic_size * 0.5f;
	
	mat[0][0] = 2.0f / (right - left);
	mat[1][1] = 2.0f / (top - bottom);
	mat[2][2] = -2.0f / (z_far - z_near);
	
	mat[3][0] = -(right + left) / (right - left);
	mat[3][1] = -(top + bottom) / (top - bottom);
	mat[3][2] = -(z_far + z_near) / (z_far - z_near);

	return mat;
}

void camera::init_as_orthographic(render_target* target, const orthographic_projection_settings& settings)
{
	m_render_target_ = target;

	float ar = settings.viewport_size.get_x() / settings.viewport_size.get_y();
	
	m_z_far_ = settings.z_far;
	m_z_near_ = settings.z_near;
	m_render_target_size_ = settings.viewport_size;
	m_orthographic_size_ = settings.size;
	
	m_projection_matrix_ = create_orthographic_projection_matrix(m_render_target_size_, m_orthographic_size_, m_z_far_, m_z_near_);

	// m_view_matrix_ = mat4_f::from_translation({0.0f, 0.0f, 0.5f});
	m_view_matrix_ = mat4_f::create_identity();
	// m_view_matrix_ = mat4_f::translate(m_view_matrix_, { (ar * settings.size * 0.5f),(settings.size * 0.5f),-3.0f });
	m_view_matrix_ = mat4_f::translate(m_view_matrix_, { 0,0,-3.0f });
}
