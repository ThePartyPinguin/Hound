#pragma once
#include "hound/core/rendering/renderer_cache/module/renderer_cache_module.h"
#include "hound/core/rendering/target/render_target.h"


class render_target_cache_module : public renderer_cache_module
{
public:
	enum target_type
	{
		WINDOW,
		VIEWPORT,
		TEXTURE
	};
	
	struct render_target_data
	{
		render_target_id id;
		target_type type;
		render_target* target_ref;
	};

	virtual render_target_id create_render_target(target_type type) = 0;	
};


#define RTC render_target_cache_module
