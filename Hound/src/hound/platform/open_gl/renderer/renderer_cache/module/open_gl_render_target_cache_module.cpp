#include "hound/hnd_pch.h"
#include "open_gl_render_target_cache_module.h"

render_target_id open_gl_render_target_cache_module::create_render_target(target_type type)
{
	switch (type)
	{
	case RTC::WINDOW:
		{
			
		}
	case RTC::VIEWPORT:
		{
		HND_CORE_LOG_ERROR("RenderTarget 'VIEWPORT' not supported!");
		return render_target_id::null();
		}
	case RTC::TEXTURE:
		{
		HND_CORE_LOG_ERROR("RenderTarget 'TEXTURE' not supported!");
		return render_target_id::null();
		}
	}
}
