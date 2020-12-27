#include "hound/hnd_pch.h"
#include "frame_buffer.h"
#include "hound/core/rendering/renderer_cache/module/frame_buffer_cache_module.h"

GET_CACHE_FUNC_IMPL(frame_buffer, frame_buffer_cache_module)
CACHED_OBJECT_CREATE_FUNC_IMPL_P1(frame_buffer, frame_buffer_cache_module, const vec2_i&, size)

void frame_buffer::init(const vec2_i& size)
{
	m_size_ = size;
}


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
