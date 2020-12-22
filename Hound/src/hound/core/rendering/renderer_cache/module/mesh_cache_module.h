#pragma once
#include "hound/core/rendering/renderer_cache/renderer_cache_module.h"
#include "hound/core/rendering/renderer_cache/cache_module_functions.h"
#include "hound/core/rendering/renderer_resource_id.h"

struct mesh_surface_data;
RENDER_CACHE_CLASS(mesh_cache_module, mesh)
{
	RENDER_CACHE_CLASS_DECL(mesh_cache_module, mesh)
	RENDER_CACHE_CREATE_FUNC_DECL(mesh)
	RENDER_CACHE_CREATE_FUNC_DECL_NAMED_P1(mesh, from_absolute_path, const char*, absolute_path)
	RENDER_CACHE_CREATE_FUNC_DECL_NAMED_P1(mesh, from_asset_path, const char*, asset_path)
	
	RENDER_CACHE_DEFAULT_DATA_STRUCT_DECL(mesh)
public:
	
	virtual mesh_data_id mesh_add_data(mesh_id mesh, const mesh_surface_data& data) = 0;
	virtual bool mesh_override_data(mesh_id mesh, mesh_data_id data_id, const mesh_surface_data& data) = 0;
	
protected:
	mesh_cache_module() = default;
	virtual ~mesh_cache_module() = default;
};