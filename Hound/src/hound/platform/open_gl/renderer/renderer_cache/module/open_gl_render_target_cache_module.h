#pragma once
#include "hound/core/rendering/renderer_cache/module/render_target_cache_module.h"

class open_gl_render_target_cache_module : public render_target_cache_module
{
public:
	render_target_id create_render_target(target_type type) override;

private:
	std::unordered_map<render_target_id, render_target_data> m_target_data_;
};

