#include "hound/hnd_pch.h"
#include "render_target.h"

void render_target::set_camera(camera* camera)
{
	m_camera_ = camera;
}

render_target::render_target()
{
	m_frame_buffer_ = nullptr;
	m_target_type_ = INVALID;
}

render_target::~render_target()
{
}
