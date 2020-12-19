#pragma once
#include "hound/core/math/math.h"
#include "hound/core/rendering/renderer_cache/module/renderer_cache_module.h"

class viewport;
class render_texture;

HND_RENDER_CACHE_CLASS_DECL(render_target_cache_module, render_target)
{
	HND_RENDER_CACHE_BASE_FUNC_DECL(render_target_cache_module)
	HND_RENDER_CACHE_CREATE_FUNC_DECL_1(viewport, const vec2_i&, size)
	HND_RENDER_CACHE_GET_FUNC_DECL(viewport)
	HND_RENDER_CACHE_DEFAULT_DATA_STRUCT_DECL(viewport)
	HND_RENDER_CACHE_CREATE_FUNC_DECL_1(render_texture, const vec2_i&, size)
	HND_RENDER_CACHE_GET_FUNC_DECL(render_texture)
	HND_RENDER_CACHE_DEFAULT_DATA_STRUCT_DECL(render_texture)

public:
	virtual std::set<render_target*> get_render_targets() = 0;
	
private:
	virtual void set_viewport_size(resource_id id, const vec2_i & size) = 0;
};