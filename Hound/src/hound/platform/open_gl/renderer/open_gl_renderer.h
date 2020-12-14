#pragma once
#include "hound/core/rendering/renderer.h"

class open_gl_renderer_cache;

class open_gl_renderer : public renderer
{
public:
	type_api get_api_type() override;

	void begin_frame() override;
	void render_indexed(shader_cache_module::shader_id shader, mesh_cache_module::mesh_id mesh) override;
	void end_frame() override;
	
	open_gl_renderer();
	virtual ~open_gl_renderer();
};

