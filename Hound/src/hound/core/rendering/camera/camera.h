#pragma once
#include "hound/core/base.h"
#include "hound/core/property.h"
#include "hound/core/rendering/render_target/render_target.h"
#include "hound/core/math/math.h"

class camera
{
public:
	enum projection
	{
		ORTHOGRAPHIC,
		PERSPECTIVE
	};
	
	HND_PROPERTY(render_target, ref<render_target>, m_render_target_)
	HND_PROPERTY(projection, projection, m_projection_)
	HND_PROPERTY(size, vec2_i, m_size_)
	HND_PROPERTY(z_near, float, m_z_near_)
	HND_PROPERTY(z_far, float, m_z_far_)
	HND_PROPERTY(fov, float, m_fov_)

	camera() = default;
	
private:
	ref<render_target> m_render_target_;
	
	projection m_projection_;
	vec2_i m_size_;
	float m_z_near_;
	float m_z_far_;
	float m_fov_;
};

