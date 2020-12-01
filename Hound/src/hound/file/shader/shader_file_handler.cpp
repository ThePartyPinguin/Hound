#include "hound/hnd_pch.h"
#include "shader_file_handler.h"

#include "hound/rendering/renderer_cache.h"
#include "raw/raw_shader_parser.h"

void shader_file_handler::deserialize(object_id instance_id, char* buffer, size_t length)
{
	raw_shader shader;
	raw_shader_parser::parse(shader, buffer, length);

}

object_id shader_file_handler::create_instance()
{
	return renderer_cache::get_instance()->shader_create();
}
