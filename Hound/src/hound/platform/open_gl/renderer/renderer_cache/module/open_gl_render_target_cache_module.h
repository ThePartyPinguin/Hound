#pragma once
#include "hound/core/rendering/renderer_cache/module/render_target_cache_module.h"
#include "hound/core/rendering/target/viewport.h"

class open_gl_render_target_cache_module : public render_target_cache_module
{
public:
	
	struct open_gl_render_target_data : render_target_data
	{
		
	};

	open_gl_render_target_cache_module();
	virtual ~open_gl_render_target_cache_module();

	render_target* get_render_target(render_target_id render_target) override;
	viewport* get_viewport(render_target_id render_target) override;

	const vec2_i& get_render_target_size(render_target_id render_target) override;
	frame_buffer_id get_target_frame_buffer(render_target_id target) override;
	std::set<render_target_id> get_render_targets() override;

	void render_target_set_frame_buffer_size(render_target_id render_target, vec2_i size) override;

	const open_gl_render_target_data& get_render_target_data(render_target_id render_target);

	render_target_id create_render_target(target_type type, const vec2_i& size) override;
		
private:	
	std::unordered_map<render_target_id, open_gl_render_target_data> m_render_target_map_;
	std::set<render_target_id> m_unique_targets_;

	render_target* create_view_port(const vec2_i& size);
};

