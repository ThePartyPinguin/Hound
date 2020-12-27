#pragma once
#include "hound/core/rendering/renderer_cache/renderer_cache_module.h"
#include "hound/core/rendering/renderer_cache/cache_module_functions.h"
#include "hound/core/rendering/camera/camera_projection_settings.h"

class render_target;
RENDER_CACHE_CLASS(camera_cache_module, camera)
{
	RENDER_CACHE_CLASS_DECL(camera_cache_module, camera)
	RENDER_CACHE_CREATE_FUNC_DECL_NAMED_P2(camera, as_orthographic, render_target*, target, const orthographic_projection_settings&, settings)
	RENDER_CACHE_CREATE_FUNC_DECL_NAMED_P2(camera, as_perspective, render_target*, target, const perspective_projection_settings&, settings)
	
	RENDER_CACHE_GET_INSTANCE_FUNC_DECL(camera)

	RENDER_CACHE_DEFAULT_DATA_STRUCT_DECL(camera)

public:
	virtual void camera_render_begin(resource_id camera) = 0;
	virtual void camera_render_end(resource_id camera) = 0;
	
	virtual void update_camera_view_matrix(resource_id camera, const mat4_f& view_matrix) = 0;
	virtual void update_camera_projection_matrix(resource_id camera, const mat4_f& projection_matrix) = 0;
};
