#pragma once
#include "hound/core/rendering/renderer_cache/module/shader_cache_module.h"

struct raw_shader
{
		
	struct raw_shader_stage
	{
		shader_cache_module::shader_stage shader_type;
		std::string shader_source;
	};

	bool is_valid;
	std::string shader_name;
	std::unordered_map<shader_cache_module::shader_stage, raw_shader_stage> shader_stages;
};

