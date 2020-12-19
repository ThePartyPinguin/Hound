#include "hound/hnd_pch.h"
#include "open_gl_renderer_cache.h"
#include "hound/platform/open_gl/renderer/renderer_cache/module/open_gl_texture_cache_module.h"
#include "hound/platform/open_gl/renderer/renderer_cache/module/open_gl_mesh_cache_module.h"
#include "hound/platform/open_gl/renderer/renderer_cache/module/open_gl_frame_buffer_cache_module.h"
#include "hound/platform/open_gl/renderer/renderer_cache/module/open_gl_shader_cache_module.h"
#include "hound/platform/open_gl/renderer/renderer_cache/module/open_gl_render_target_cache_module.h"

open_gl_renderer_cache::open_gl_renderer_cache()
{
	s_instance_ = this;

	register_module<mesh_cache_module, open_gl_mesh_cache_module>();
	register_module<texture_cache_module, open_gl_texture_cache_module>();
	register_module<shader_cache_module, open_gl_shader_cache_module>();
	register_module<frame_buffer_cache_module, open_gl_frame_buffer_cache_module>();
	register_module<render_target_cache_module, open_gl_render_target_cache_module>();
}

open_gl_renderer_cache::~open_gl_renderer_cache()
{
}
