#pragma once

#include "hound/core/rendering/renderer_cache/module/shader_cache_module.h"
#include "hound/platform/open_gl/open_gl_core.h"

class open_gl_shader_cache_module : public shader_cache_module
{
public:
	struct gl_shader_data : shader_data
	{
		gl_object_id gl_shader_program_id;

		struct stage_info
		{
			gl_object_id stage_id;
			std::string source;
		};

		std::unordered_map<shader::stage, stage_info> program_stage_info;
	};

	shader* get_shader_object(shader_id shader) override;
	shader_id shader_create() override;
	void shader_set_source(shader_id shader, shader::stage stage, const std::string& source) override;
	const std::string& shader_get_source(shader_id shader, shader::stage stage) override;
	bool shader_finalize(shader_id shader) override;
	gl_object_id get_shader_program_id(shader_id shader);

	shader_id get_standard_screen_shader();
	
	open_gl_shader_cache_module();
	virtual ~open_gl_shader_cache_module();

private:
	shader_id m_standard_screen_shader_;

	std::unordered_map<shader_id, gl_shader_data> m_gl_shader_map_;

	void create_standard_screen_shader();
};

