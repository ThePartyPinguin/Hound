#pragma once
#include "hound/core/object/object.h"

struct mesh_surface_data;

HND_OBJECT_CLASS_DECL(mesh, mesh_cache_module)
{
	HND_OBJECT_CLASS_FUNC_DECL(mesh, mesh_cache_module)

public:
	void add_surface_data(const mesh_surface_data& data);
};

