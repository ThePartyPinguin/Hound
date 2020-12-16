#pragma once
#include "hound/core/rendering/renderer_cache/module/texture_cache_module.h"
#include "hound/platform/open_gl/open_gl_core.h"

class open_gl_texture_cache_module : public texture_cache_module 
{
public:	
	struct gl_texture_data : texture_data
	{
		gl_object_id gl_texture_object_id;
	};
	
	texture_id texture_create_2d(const vec2_i& size) override;
	void texture_set_2d_size(texture_id texture, const vec2_i& size) override;
	void texture_set_2d_filter_mode(texture_id texture, texture_filter_mode mode) override;
	void texture_set_2d_wrap_mode(texture_id texture, texture_wrap_mode h, texture_wrap_mode v) override;
	void texture_set_2d_data(texture_id texture, unsigned char* tex_data_buffer) override;
	unsigned char* get_texture_2d_data(texture_id texture, vec2_i& size) override;
	void texture_generate_2d_mip_map(texture_id texture) override;
	bool texture_has_mip_map(texture_id texture) override;

	void bind_texture(texture_id texture);
	void un_bind_texture(texture_id texture);
	
	const gl_texture_data& get_gl_texture_data(texture_id texture);

	open_gl_texture_cache_module();
	virtual ~open_gl_texture_cache_module();
	
private:
	std::unordered_map<texture_id, gl_texture_data> m_gl_texture_map_;

	GLint texture_get_gl_text_type(texture_type type);
	GLint texture_get_gl_wrap_mode(texture_wrap_mode mode);
	GLint texture_get_gl_filter_mode(texture_filter_mode mode);
};

#define HND_GL_TCM open_gl_texture_cache_module