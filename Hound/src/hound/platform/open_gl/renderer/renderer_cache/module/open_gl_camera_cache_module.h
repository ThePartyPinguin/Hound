#pragma once
#include "hound/core/rendering/renderer_cache/module/camera_cache_module.h"
#include "hound/platform/open_gl/open_gl_core.h"

class open_gl_camera_cache_module : public camera_cache_module
{
	friend class open_gl_renderer_cache;
public:
	struct gl_camera_matrix_data
	{
									//base alignment	//alignment offset
		mat4_f view_matrix;			//4 * 16 (4 * vec4)	//0
		mat4_f projection_matrix;	//4 * 16 (4 * vec4)	//64
									//Total size to reserve in ubo (4 * 16) + (4 * 16) = 128
		const static size_t gl_reserve_size = 128;
		const static size_t view_matrix_offset = 0;
		const static size_t view_matrix_size = 64;
		const static size_t projection_matrix_offset = 64;
		const static size_t projection_matrix_size = 64;
	};
	
	struct gl_camera_data : internal_camera_data
	{
		//OpenGL uniform buffer object containing the view and projection matrix
		gl_object_id gl_matrix_ubo_id;
		render_target* render_target;
		gl_camera_matrix_data matrix_data;
	};

	void on_create_instance_as_orthographic(camera* instance, render_target* target, const orthographic_projection_settings& settings) override;
	void on_create_instance_as_perspective(camera* instance, render_target* target, const perspective_projection_settings& settings) override;

	void update_camera_view_matrix(resource_id camera, const mat4_f& view_matrix) override;
	void update_camera_projection_matrix(resource_id camera, const mat4_f& projection_matrix) override;

	void camera_render_begin(resource_id camera) override;
	void camera_render_end(resource_id camera) override;

	void init() override;
	
	open_gl_camera_cache_module();
	~open_gl_camera_cache_module();
private:
	gl_camera_data& get_camera_data(resource_id camera);

	void set_gl_ubo_data(resource_id camera, size_t offset, size_t size, void* data);
	
	std::unordered_map<resource_id, gl_camera_data> m_camera_data_map_;
};