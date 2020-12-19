#include "hound/hnd_pch.h"
#include "frame_buffer.h"
#include "hound/core/rendering/renderer_cache/module/frame_buffer_cache_module.h"

HND_OBJECT_CLASS_FUNC_IMPL_1(frame_buffer, frame_buffer_cache_module, const vec2_i&)

void frame_buffer::bind()
{
	get_cache()->bind_frame_buffer(get_object_id());
}

void frame_buffer::un_bind()
{
	get_cache()->un_bind_frame_buffer(get_object_id());
}

void frame_buffer::on_set_size(const vec2_i& size)
{
	get_cache()->frame_buffer_set_size(get_object_id(), size);
}
