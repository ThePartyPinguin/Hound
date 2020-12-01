#pragma once
struct raw_shader
{
	enum class shader_stage_type
	{
		INVALID,
		VERTEX,
		GEOMETRY,
		TESSELLATION,
		FRAGMENT,
		COMPUTE
	};
	
	struct shader_stage
	{
		shader_stage_type shader_type;
		std::string shader_source;
	};

	bool is_valid;
	std::string shader_name;
	std::unordered_map<shader_stage_type, shader_stage> shader_stages;
};

