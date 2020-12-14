#pragma once
#include "hound/core/rendering/renderer_cache/module/mesh_cache_module.h"
#include "hound/platform/open_gl/open_gl_core.h"

class open_gl_mesh_cache_module : public mesh_cache_module
{
public:
	struct gl_mesh_data : mesh_data
	{
		gl_object_id gl_array_buffer_id;
		gl_object_id gl_element_buffer_id;
		mesh_data_id id;
		mesh_data data;
	};

	struct gl_mesh_object
	{
		gl_object_id gl_vertex_array_id;
		mesh_data_id data_count;
		std::map<mesh_data_id, gl_mesh_data> sub_objects;
	};
	
	mesh_id mesh_create() override;;
	mesh_data_id mesh_add_data(mesh_id mesh, const mesh_data& data) override;
	bool mesh_override_data(mesh_id mesh, mesh_data_id data_id, const mesh_data& data) override;
	const gl_mesh_object& get_gl_mesh_object(mesh_id mesh);
	
private:
	std::unordered_map<mesh_id, gl_mesh_object> m_gl_mesh_map_;
};

