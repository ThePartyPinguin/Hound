#pragma once
#include "hound/core/object/texture/texture.h"
#include "hound/core/rendering/renderer_cache/cache_object_functions.h"

CACHED_DERIVED_OBJECT(texture2d, texture, texture_cache_module)
{
	CACHED_OBJECT_DECL(texture, texture_cache_module)
	CACHED_OBJECT_CREATE_FUNC_DECL_NAMED_P1(texture2d, from_size_2d, const vec2_i&, size)
	CACHED_OBJECT_CREATE_FUNC_DECL_NAMED_P1(texture2d, from_absolute_path_2d, const char*, absolute_path)
	CACHED_OBJECT_CREATE_FUNC_DECL_NAMED_P1(texture2d, from_asset_path_2d, const char*, asset_path)
};

