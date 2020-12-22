#pragma once
#include "hound/core/rendering/renderer_cache/cache_object_functions.h"
struct mesh_surface_data;

CACHED_OBJECT(mesh, mesh_cache_module)
{
	CACHED_OBJECT_DECL(mesh, mesh_cache_module)
	CACHED_OBJECT_CREATE_FUNC_DECL(mesh)
	CACHED_OBJECT_CREATE_FUNC_DECL_NAMED_P1(mesh, from_absolute_path, const char*, absolute_path)
	CACHED_OBJECT_CREATE_FUNC_DECL_NAMED_P1(mesh, from_asset_path, const char*, asset_path)

public:
	void add_surface_data(const mesh_surface_data& data);

protected:
	mesh() = default;
	~mesh() = default;
};

