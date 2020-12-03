#pragma once
#include "hound/platform/open_gl/open_gl_core.h"
#include "hound/core/rendering/renderer_cache.h"

class open_gl_renderer_cache : public renderer_cache 
{
public:

	struct gl_shader_data
	{
		gl_object_id gl_shader_program_id;

		struct stage_info
		{
			gl_object_id stage_id;
			std::string source;
		};

		std::unordered_map<shader::stage, stage_info> program_stage_info;
	};
	
	shader_id shader_create() override;
	void shader_set_source(shader_id shader, shader::stage stage, const std::string& source) override;
	const std::string& shader_get_source(shader_id shader, shader::stage stage) override;
	bool shader_finalize(shader_id shader) override;
	gl_object_id get_shader_program_id(shader_id shader);

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
	
	mesh_id mesh_create() override;
	mesh_data_id mesh_add_data(mesh_id mesh, const mesh_data& data) override;
	bool mesh_override_data(mesh_id mesh, mesh_data_id data_id, const mesh_data& data) override;
	gl_object_id get_mesh_vertex_array_object(mesh_id mesh);

	struct gl_texture_data : texture_data
	{
		gl_object_id gl_texture_object_id;
	};

	texture_id texture_create_2d() override;
	void texture_set_2d_size(texture_id texture, const vec2_i& size) override;
	void texture_set_2d_filter_mode(texture_id texture, texture_filter_mode mode) override;
	void texture_set_2d_wrap_mode(texture_id texture, texture_wrap_mode h, texture_wrap_mode v) override;
	void texture_set_2d_data(texture_id texture, unsigned char* raw_data) override;
	unsigned char* get_texture_2d_data(texture_id texture, vec2_i& size) override;
	void texture_generate_2d_mip_map(texture_id texture) override;
	bool texture_has_mip_map(texture_id texture) override;
	
	struct gl_frame_buffer_data : frame_buffer_data
	{
		gl_object_id gl_frame_buffer_object_id;
		texture_id color_buffer_texture_id;
	};

	frame_buffer_id frame_buffer_create() override;
	void frame_buffer_set_size(frame_buffer_id frame_buffer, const vec2_i& size) override;
	const vec2_i& frame_buffer_get_size(frame_buffer_id frame_buffer) override;
	gl_object_id get_frame_buffer_object_id(frame_buffer_id frame_buffer);
	
	open_gl_renderer_cache();
	virtual ~open_gl_renderer_cache();
		
private:
	
	std::unordered_map<shader_id, gl_shader_data> m_gl_shader_map_;
	std::unordered_map<mesh_id, gl_mesh_object> m_gl_mesh_map_;
	std::unordered_map<texture_id, gl_texture_data> m_gl_texture_map_;
	std::unordered_map<frame_buffer_id, gl_frame_buffer_data> m_gl_frame_buffer_map_;

	GLint texture_get_gl_text_type(texture_type type);
	GLint texture_get_gl_wrap_mode(texture_wrap_mode mode);
	GLint texture_get_gl_filter_mode(texture_filter_mode mode);
};

