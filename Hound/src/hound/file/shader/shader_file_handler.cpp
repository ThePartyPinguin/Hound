#include "hound/hnd_pch.h"
#include "shader_file_handler.h"

#include "hound/rendering/renderer_cache.h"
#include "raw/raw_shader_parser.h"

void shader_file_handler::deserialize(object_id instance_id, char* buffer, size_t length)
{
	raw_shader shader;
	raw_shader_parser::parse(shader, buffer, length);

	if (!shader.is_valid)
		return;
	
	for (const auto& pair : shader.shader_stages)
	{
		if(pair.first == shader::stage::INVALID)
			continue;
		
		renderer_cache::get_instance()->shader_set_source(instance_id, pair.first, pair.second.shader_source);
	}

	renderer_cache::get_instance()->shader_finalize(instance_id);
}

object_id shader_file_handler::create_instance()
{
	return renderer_cache::get_instance()->shader_create();
}
