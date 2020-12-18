#include "hound/hnd_pch.h"
#include "monitor.h"

void monitor::add_video_mode(const video_mode& mode)
{
	m_video_modes_.insert(mode);
}
