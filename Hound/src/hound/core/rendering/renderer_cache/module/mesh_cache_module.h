#pragma once
#include "hound/core/rendering/renderer_cache/module/renderer_cache_module.h"
#include "hound/core/rendering/renderer_resource_id.h"

struct mesh_surface_data;
HND_RENDER_CACHE_CLASS_DECL(mesh_cache_module, mesh)
{
	HND_RENDER_CACHE_BASE_FUNC_DECL(mesh_cache_module, mesh)
public:
	virtual mesh_id mesh_create() = 0;
	virtual mesh_data_id mesh_add_data(mesh_id mesh, const mesh_surface_data& data) = 0;
	virtual bool mesh_override_data(mesh_id mesh, mesh_data_id data_id, const mesh_surface_data& data) = 0;
	
protected:
	mesh_cache_module() = default;
	virtual ~mesh_cache_module() = default;
};