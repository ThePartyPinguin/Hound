#pragma once
#include "hound/core/rendering/target/render_target.h"

HND_OBJECT_DERIVED_CLASS_DECL(render_texture, render_target, render_target_cache_module)
{
	HND_OBJECT_CLASS_FUNC_DECL(render_texture, render_target_cache_module)

public:
	render_texture();
	~render_texture() = default;

	void begin_frame() override;
	void end_frame() override;
};

