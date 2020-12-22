#pragma once
#include "hound/core/rendering/target/render_target.h"
#include "hound/core/rendering/renderer_cache/cache_object_functions.h"

CACHED_DERIVED_OBJECT(render_texture, render_target, render_target_cache_module)
{
	CACHED_OBJECT_DECL(render_texture, render_target_cache_module)
	CACHED_OBJECT_CREATE_FUNC_DECL_P1(render_texture, const vec2_i&, size)

public:
	render_texture();
	~render_texture() = default;

	void begin_frame() override;
	void end_frame() override;
};

