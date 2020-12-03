#pragma once
#include "hound/core/object/shader/shader.h"
struct raw_shader
{
		
	struct raw_shader_stage
	{
		shader::stage shader_type;
		std::string shader_source;
	};

	bool is_valid;
	std::string shader_name;
	std::unordered_map<shader::stage, raw_shader_stage> shader_stages;
};

