#include "hound/hnd_pch.h"
#include "shader_file_handler.h"

#include "hound/core/rendering/renderer_cache.h"
#include "hound/core/rendering/renderer_cache/module/shader_cache_module.h"
#include "raw/raw_shader_parser.h"

void shader_file_handler::deserialize(object_id instance_id, char* buffer, size_t length)
{
	raw_shader shader;
	raw_shader_parser::parse(shader, buffer, length);

	if (!shader.is_valid)
		return;

	shader_cache_module* shader_cache = renderer_cache::shader_cache();
	
	for (const auto& pair : shader.shader_stages)
	{
		if(pair.first == shader::stage::INVALID)
			continue;
		
		shader_cache->shader_set_source(instance_id, pair.first, pair.second.shader_source);
	}

	shader_cache->shader_finalize(instance_id);
}

object_id shader_file_handler::create_instance()
{
	return renderer_cache::shader_cache()->shader_create();
}
