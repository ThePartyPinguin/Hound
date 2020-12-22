#include "hound/hnd_pch.h"
#include "texture.h"
#include "hound/core/rendering/renderer_cache/module/texture_cache_module.h"
#include "hound/core/object/texture/texture2d.h"

GET_CACHE_FUNC_IMPL(texture, texture_cache_module)

texture2d* texture::create_texture_2d(const vec2_i& size)
{
	return texture2d::create_from_size_2d(size);
}

texture2d* texture::create_texture_2d_from_absolute_path(const char* absolute_path)
{
	return texture2d::create_from_absolute_path_2d(absolute_path);
}

texture2d* texture::create_texture_2d_from_asset_path(const char* asset_path)
{
	return texture2d::create_from_asset_path_2d(asset_path);
}
