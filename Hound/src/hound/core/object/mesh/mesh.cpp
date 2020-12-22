#include "hound/hnd_pch.h"
#include "mesh.h"
#include "hound/core/rendering/renderer_cache/module/mesh_cache_module.h"

GET_CACHE_FUNC_IMPL(mesh, mesh_cache_module)
CACHED_OBJECT_CREATE_FUNC_IMPL(mesh, mesh_cache_module)
CACHED_OBJECT_CREATE_FUNC_IMPL_NAMED_P1(mesh, mesh_cache_module, from_absolute_path, const char*, absolute_path)
CACHED_OBJECT_CREATE_FUNC_IMPL_NAMED_P1(mesh, mesh_cache_module, from_asset_path, const char*, asset_path)

void mesh::add_surface_data(const mesh_surface_data& data)
{
	get_cache()->mesh_add_data(get_object_id(), data);
}
