#pragma once
#include <string>
#include "hound/core/object/shader/shader.h"
#include "hound/core/rendering/renderer_cache/module/renderer_cache_module.h"
#include "hound/core/rendering/renderer_resource_id.h"

class shader_cache_module : public renderer_cache_module
{
public:
	struct shader_data
	{
		shader* shader_object;
	};
	
	virtual shader_id shader_create() = 0;
	virtual shader* get_shader_object(shader_id shader) = 0;
	virtual void shader_set_source(shader_id shader, shader::stage stage, const std::string& source) = 0;
	virtual const std::string& shader_get_source(shader_id shader, shader::stage stage) = 0;
	virtual bool shader_finalize(shader_id shader) = 0;

protected:
	shader_cache_module() = default;
	virtual ~shader_cache_module() = default;
};