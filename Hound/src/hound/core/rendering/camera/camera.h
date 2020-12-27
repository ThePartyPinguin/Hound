#pragma once
#include "hound/core/base.h"
#include "hound/core/property.h"
#include "hound/core/math/math.h"
#include "hound/core/rendering/target/render_target.h"
#include "hound/core/rendering/renderer_cache/cache_object_functions.h"

struct perspective_projection_settings;
struct orthographic_projection_settings;
enum class camera_projection;

CACHED_OBJECT(camera, camera_cache_module)
{
	friend class render_target;
	friend class viewport;
	friend class render_texture;
	
	CACHED_OBJECT_DECL(camera, camera_cache_module)
	CACHED_OBJECT_CREATE_FUNC_DECL_NAMED_P2(camera, as_orthographic, render_target*, target, const orthographic_projection_settings&, settings)
	CACHED_OBJECT_CREATE_FUNC_DECL_NAMED_P2(camera, as_perspective, render_target*, target, const perspective_projection_settings&, settings)
	
public:
	HND_PROPERTY_PTR_READ_ONLY(render_target, render_target, m_render_target_)
	HND_PROPERTY(projection, camera_projection, m_projection_)
	HND_PROPERTY(projection_matrix, mat4_f, m_projection_matrix_)
	HND_PROPERTY(view_matrix, mat4_f, m_view_matrix_)
	HND_PROPERTY_CALLBACK(render_target_size, vec2_f, m_render_target_size_, on_render_target_size_set)
	HND_PROPERTY_CALLBACK(orthograpic_size, float, m_orthographic_size_, on_orthographic_size_set)
	HND_PROPERTY(z_near, float, m_z_near_)
	HND_PROPERTY(z_far, float, m_z_far_)
	HND_PROPERTY(pers_fov, float, m_fov_)

	camera() = default;
	
private:	
	render_target* m_render_target_;
	
	camera_projection m_projection_;
	vec2_f m_render_target_size_;
	float m_orthographic_size_;
	float m_z_near_;
	float m_z_far_;
	float m_fov_;
	mat4_f m_projection_matrix_;
	mat4_f m_view_matrix_;

	void begin_frame();
	void end_frame();

	void on_render_target_size_set(const vec2_f& size);
	void on_orthographic_size_set(const float& size);

	static mat4_f create_orthographic_projection_matrix(const vec2_f& target_size, float orthographic_size, float z_far, float z_near);
};

