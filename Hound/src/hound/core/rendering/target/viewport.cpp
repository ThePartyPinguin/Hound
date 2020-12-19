#include "hound/hnd_pch.h"
#include "viewport.h"

#include "hound/core/window/window.h"
#include "hound/core/rendering/renderer_cache/module/viewport_cache_module.h"

HND_OBJECT_CLASS_FUNC_IMPL(viewport, viewport_cache_module)

void viewport::begin_frame()
{
	m_owner_window_->begin_frame();
}

void viewport::end_frame()
{
	m_owner_window_->end_frame();
}

void viewport::on_set_size(const vec2_i& size)
{
	get_cache()->set_viewport_size(get_object_id(), size);
}
