#pragma once
#include "hound/core/rendering/renderer_cache/module/renderer_cache_module.h"

HND_RENDER_CACHE_CLASS_DECL(viewport_cache_module, viewport)
{
	HND_RENDER_CACHE_BASE_FUNC_DECL(viewport_cache_module, viewport)

	struct viewport_data
	{
		resource_id id;
		viewport* viewport_handle;
	};

	virtual void set_viewport_size(resource_id viewport, const vec2_i & size) = 0;
};
