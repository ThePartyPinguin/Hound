#include "hound/hnd_pch.h"
#include "texture2d.h"
#include "hound/core/rendering/renderer_cache/module/texture_cache_module.h"

CACHED_OBJECT_CREATE_FUNC_IMPL_NAMED_P1(texture2d, texture_cache_module, from_size_2d, const vec2_i&, size)
CACHED_OBJECT_CREATE_FUNC_IMPL_NAMED_P1(texture2d, texture_cache_module, from_absolute_path_2d, const char*, absolute_path)
CACHED_OBJECT_CREATE_FUNC_IMPL_NAMED_P1(texture2d, texture_cache_module, from_asset_path_2d, const char*, asset_path)
CACHED_OBJECT_DEFAULT_INIT_FUNC_IMPL_NAMED_P1(texture2d, texture_cache_module, from_size_2d, const vec2_i&, size)
CACHED_OBJECT_DEFAULT_INIT_FUNC_IMPL_NAMED_P1(texture2d, texture_cache_module, from_absolute_path_2d, const char*, absolute_path)
CACHED_OBJECT_DEFAULT_INIT_FUNC_IMPL_NAMED_P1(texture2d, texture_cache_module, from_asset_path_2d, const char*, asset_path)