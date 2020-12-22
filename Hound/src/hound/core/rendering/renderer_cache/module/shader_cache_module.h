#pragma once
#include <string>
#include "hound/core/rendering/renderer_cache/renderer_cache_module.h"
#include "hound/core/rendering/renderer_cache/cache_module_functions.h"
#include "hound/core/rendering/renderer_resource_id.h"


RENDER_CACHE_CLASS(shader_cache_module, shader)
{
	RENDER_CACHE_CLASS_DECL(shader_cache_module, shader)
	RENDER_CACHE_CREATE_FUNC_DECL_NAMED_P1(shader, by_name, const char*, name)
	RENDER_CACHE_CREATE_FUNC_DECL_NAMED_P1(shader, from_absolute_path, const char*, absolute_path)
	RENDER_CACHE_CREATE_FUNC_DECL_NAMED_P1(shader, from_asset_path, const char*, asset_path)
	
public:
	friend class shader;
	
	enum shader_stage
	{
		SHADER_STAGE_INVALID,
		SHADER_STAGE_VERTEX,
		SHADER_STAGE_GEOMETRY,
		SHADER_STAGE_TESSELLATION,
		SHADER_STAGE_FRAGMENT,
		SHADER_STAGE_COMPUTE
	};
	
	struct shader_data
	{
		std::string name;
		shader* shader_object;
	};
	//
	// virtual shader_id shader_create(const std::string& name) = 0;
	// virtual shader_id shader_create_from_source_asset(const std::string& source_path) = 0;
	// virtual shader_id shader_create_from_absolute_file(const std::string& source_path) = 0;
	virtual shader* get_shader_object(shader_id shader) = 0;
	virtual void shader_set_source(shader_id shader, shader_stage stage, const std::string& source) = 0;
	virtual const std::string& shader_get_source(shader_id shader, shader_stage stage) = 0;
	virtual bool shader_finalize(shader_id shader) = 0;

	virtual void use_shader(shader_id shader) = 0;
	virtual void set_uniform_float(shader_id shader, const char* name, float value) = 0;
	
protected:

	virtual const shader_data& get_shader_data(shader_id shader) = 0;
	
	static constexpr const char* shader_file_extension = ".shad";

	shader_cache_module() = default;
	virtual ~shader_cache_module() = default;
};
