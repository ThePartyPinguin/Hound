#pragma once
#include "hound/core/base.h"
#include "hound/core/object/object.h"
#include "hound/core/scene/camera/camera.h"

class viewport : public render_target
{
public:
	ref<viewport> create(const ref<camera>& camera, vec2_i size);
	
	HND_PROPERTY_READ_ONLY(camera, ref<camera>, m_camera_)

	void set_size();

	viewport() = default;
	
protected:
	
	ref<camera> m_camera_;
};
