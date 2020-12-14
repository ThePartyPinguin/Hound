#include "hound/hnd_pch.h"
#include "render_target.h"
#include "hound/core/rendering/renderer_cache.h"

render_target::render_target()
{
}

render_target::~render_target()
{
}

void render_target::frame_render_start()
{
	on_frame_render_start();
}

void render_target::frame_render_end()
{
	on_frame_render_end();
}

void render_target::on_frame_render_start()
{
}

void render_target::on_frame_render_end()
{
}
