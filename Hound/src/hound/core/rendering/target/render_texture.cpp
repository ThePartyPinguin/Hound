#include "hound/hnd_pch.h"
#include "render_texture.h"

#include "hound/core/rendering/renderer_cache/module/render_target_cache_module.h"

GET_CACHE_FUNC_IMPL(render_texture, render_target_cache_module)
CACHED_OBJECT_CREATE_FUNC_IMPL_P1(render_texture, render_target_cache_module, const vec2_i&, size)

render_texture::render_texture()
{
	m_target_type_ = RENDER_TEXTURE;
}

void render_texture::begin_frame()
{
}

void render_texture::end_frame()
{
}

bool render_texture::should_render()
{
	return true;
}
