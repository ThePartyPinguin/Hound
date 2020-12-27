#pragma once
#include "hound/core/rendering/renderer_cache/renderer_cache.h"
#include "hound/platform/open_gl/renderer/renderer_cache/module/open_gl_texture_cache_module.h"
#include "hound/platform/open_gl/renderer/renderer_cache/module/open_gl_mesh_cache_module.h"
#include "hound/platform/open_gl/renderer/renderer_cache/module/open_gl_frame_buffer_cache_module.h"
#include "hound/platform/open_gl/renderer/renderer_cache/module/open_gl_shader_cache_module.h"
#include "hound/platform/open_gl/renderer/renderer_cache/module/open_gl_render_target_cache_module.h"

class open_gl_renderer_cache : public renderer_cache 
{
public:
	HND_RENDER_CACHE_MODULE_ACCESS_FUNCTION(gl_texture, open_gl_texture_cache_module)
	HND_RENDER_CACHE_MODULE_ACCESS_FUNCTION(gl_shader, open_gl_shader_cache_module)
	HND_RENDER_CACHE_MODULE_ACCESS_FUNCTION(gl_mesh, open_gl_mesh_cache_module)
	HND_RENDER_CACHE_MODULE_ACCESS_FUNCTION(gl_frame_buffer, open_gl_frame_buffer_cache_module)
	HND_RENDER_CACHE_MODULE_ACCESS_FUNCTION(gl_render_target, open_gl_render_target_cache_module)

	void init() override;
	
	open_gl_renderer_cache();
	virtual ~open_gl_renderer_cache();
};

