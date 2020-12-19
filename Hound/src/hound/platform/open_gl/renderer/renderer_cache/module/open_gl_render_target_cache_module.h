#pragma once
#include "hound/core/rendering/renderer_cache/module/render_target_cache_module.h"
#include "hound/platform/open_gl/open_gl_core.h"

class open_gl_render_target_cache_module : public render_target_cache_module
{
public:
	struct gl_viewport_data : internal_viewport_data
	{
	};

	void on_create_instance(viewport* instance, const vec2_i& size) override;
	void set_viewport_size(resource_id id, const vec2_i& size) override;
	
	struct gl_render_texture_data : internal_render_texture_data
	{
		gl_object_id gl_texture_id;
	};

	void on_create_instance(render_texture* instance, const vec2_i& size) override;
	
	std::set<render_target*> get_render_targets() override;
	
	open_gl_render_target_cache_module();
	virtual ~open_gl_render_target_cache_module();
	
private:	
	std::unordered_map<render_target_id, gl_viewport_data> m_viewport_data_map_;
	std::unordered_map<render_target_id, gl_render_texture_data> m_render_texture_data_map_;
	std::set<render_target*> m_unique_targets_;
};

