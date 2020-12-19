#pragma once
#include "hound/core/rendering/renderer_cache/module/viewport_cache_module.h"

class open_gl_viewport_cache_module : public viewport_cache_module
{
	void set_viewport_size(resource_id viewport, const vec2_i& size) override;
};

