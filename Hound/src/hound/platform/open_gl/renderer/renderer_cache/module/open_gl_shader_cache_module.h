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
			gl_object_id gl_shader_stage_id;
			std::string source;
		};

		std::unordered_map<shader_stage, stage_info> program_stage_info;
	};

	void on_create_instance_by_name(shader* instance, const char* name) override;
	void on_create_instance_from_absolute_path(shader* instance, const char* absolute_path) override;
	void on_create_instance_from_asset_path(shader* instance, const char* asset_path) override;
	
	shader* get_shader_object(shader_id shader) override;
	// shader_id shader_create(const std::string& name) override;
	// shader_id shader_create_from_source_asset(const std::string& source_path) override;
	// shader_id shader_create_from_absolute_file(const std::string& source_path) override;
	void shader_set_source(shader_id shader, shader_stage stage, const std::string& source) override;
	const std::string& shader_get_source(shader_id shader, shader_stage stage) override;
	bool shader_finalize(shader_id shader) override;
	gl_object_id get_shader_program_id(shader_id shader);

	const shader_data& get_shader_data(shader_id shader) override;
	
	shader_id get_standard_screen_shader();

	void use_shader(shader_id shader) override;
	void set_uniform_float(shader_id shader, const char* name, float value) override;
	void set_uniform_mat4_f(shader_id shader, const char* name, const mat4_f& value) override;
	
	void init() override;
	
	open_gl_shader_cache_module();
	virtual ~open_gl_shader_cache_module();

private:
	shader_id m_standard_screen_shader_;

	std::unordered_map<shader_id, gl_shader_data> m_gl_shader_map_;

	void create_standard_screen_shader();
	
};

