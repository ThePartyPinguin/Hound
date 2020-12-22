#pragma once
#include "hound/core/math/math.h"
#include "hound/core/rendering/renderer_cache/renderer_cache_module.h"
#include "hound/core/rendering/renderer_cache/cache_module_functions.h"

class viewport;
class render_texture;

RENDER_CACHE_CLASS(render_target_cache_module, render_target)
{
	RENDER_CACHE_CLASS_DECL(render_target_cache_module, render_target)
	RENDER_CACHE_FRIEND_DECL(viewport)
	RENDER_CACHE_CREATE_FUNC_DECL_P1(viewport, const vec2_i&, size)
	RENDER_CACHE_DEFAULT_DATA_STRUCT_DECL(viewport)

	RENDER_CACHE_FRIEND_DECL(render_texture)
	RENDER_CACHE_CREATE_FUNC_DECL_P1(render_texture, const vec2_i&, size)
	RENDER_CACHE_DEFAULT_DATA_STRUCT_DECL(render_texture)
	

public:
	virtual std::set<render_target*> get_render_targets() = 0;
	
private:
	virtual void set_viewport_size(resource_id id, const vec2_i & size) = 0;
};