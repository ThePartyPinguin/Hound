#pragma once
#include <vector>
#include "hound/core/rendering/renderer_cache/module/renderer_cache_module.h"

class mesh_cache_module : public renderer_cache_module
{
public:
	typedef object_id mesh_id;
	typedef uint32_t mesh_data_id;

	struct mesh_data
	{
		struct vertex
		{
			vec3_f position;
			vec3_f normal;
			vec2_f uv;
		};

		std::vector<vertex> vertices;
		std::vector<uint32_t> indices;
	};

	virtual mesh_id mesh_create() = 0;
	virtual mesh_data_id mesh_add_data(mesh_id mesh, const mesh_data& data) = 0;
	virtual bool mesh_override_data(mesh_id mesh, mesh_data_id data_id, const mesh_data& data) = 0;
	
protected:
	mesh_cache_module() = default;
	virtual ~mesh_cache_module() = default;
};