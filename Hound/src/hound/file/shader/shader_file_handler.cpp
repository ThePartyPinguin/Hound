#include "hound/hnd_pch.h"
#include "shader_file_handler.h"

#include "hound/core/rendering/renderer_cache.h"
#include "hound/core/rendering/renderer_cache/module/shader_cache_module.h"
#include "raw/raw_shader_parser.h"

object_id shader_file_handler::deserialize(char* buffer, size_t length)
{
	raw_shader raw;
	raw_shader_parser::parse(raw, buffer, length);

	if (!raw.is_valid)
		return object_id::null();

	shader_cache_module* shader_cache = renderer_cache::shader_cache();

	const shader_id instance_id = shader_cache->shader_create(raw.shader_name);
	
	for (const auto& pair : raw.shader_stages)
	{
		if(pair.first == shader_cache_module::SHADER_STAGE_INVALID)
			continue;
		
		shader_cache->shader_set_source(instance_id, pair.first, pair.second.shader_source);
	}

	shader_cache->shader_finalize(instance_id);

	return instance_id;
}
