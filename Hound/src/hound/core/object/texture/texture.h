#pragma once
#include "hound/core/rendering/renderer_cache/cache_object_functions.h"

class texture2d;
CACHED_OBJECT(texture, texture_cache_module)
{
	CACHED_OBJECT_DECL(texture, texture_cache_module)

public:
	static texture2d* create_texture_2d(const vec2_i & size);
	static texture2d* create_texture_2d_from_absolute_path(const char* absolute_path);
	static texture2d* create_texture_2d_from_asset_path(const char* asset_path);
};

