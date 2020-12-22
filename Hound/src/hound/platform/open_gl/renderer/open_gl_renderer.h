#pragma once
#include "hound/core/rendering/renderer.h"

class open_gl_renderer_cache;

class open_gl_renderer : public renderer
{
public:
	type_api get_api_type() override;

	void begin_frame(render_target* render_target) override;
	void end_frame(render_target* render_target) override;
	void render_indexed(shader_id shader_id, mesh_id mesh) override;
	
	open_gl_renderer();
	virtual ~open_gl_renderer();
};

