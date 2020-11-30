#pragma once
#include "hound/platform/open_gl/open_gl_core.h"
#include "hound/rendering/renderer_cache.h"

class open_gl_renderer_cache : public renderer_cache 
{
public:

	struct gl_shader_data
	{
		gl_object_id program_id;

		struct stage_info
		{
			gl_object_id stage_id;
			std::string source;
		};

		std::unordered_map<shader_stage, stage_info> program_stage_info;
	};

	struct gl_mesh_data : mesh_data
	{
		gl_object_id array_buffer_id;
		gl_object_id element_buffer_id;
		mesh_data_id id;
		mesh_data data;
	};

	struct gl_mesh_object
	{
		gl_object_id vertex_array_id;

		mesh_data_id data_count;

		std::map<mesh_data_id, gl_mesh_data> sub_objects;
	};
	
	shader_id shader_create() override;
	void shader_set_source(shader_id shader, shader_stage stage, const std::string& source) override;
	const std::string& shader_get_source(shader_id shader, shader_stage stage) override;
	bool shader_finalize(shader_id shader) override;
	gl_object_id get_shader_program_id(shader_id shader);

	
	mesh_id mesh_create() override;
	mesh_data_id mesh_add_data(mesh_id mesh, const mesh_data& data) override;
	bool mesh_override_data(mesh_id mesh, mesh_data_id data_id, const mesh_data& data) override;
	gl_object_id get_mesh_vertex_array_object(mesh_id mesh);

	open_gl_renderer_cache();
	virtual ~open_gl_renderer_cache();
		
private:
	
	std::unordered_map<shader_id, gl_shader_data> m_gl_shader_map_;
	
	std::unordered_map<mesh_id, gl_mesh_object> m_gl_mesh_map_;
};

