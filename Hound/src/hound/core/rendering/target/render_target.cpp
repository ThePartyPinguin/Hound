#include "hound/hnd_pch.h"
#include "render_target.h"

render_target::render_target()
{
	m_frame_buffer_ = nullptr;
	m_target_type_ = INVALID;
}

render_target::~render_target()
{
}
