#include "hound/hnd_pch.h"
#include "viewport.h"

#include "hound/core/window/window.h"
void viewport::begin_frame()
{
	m_owner_window_->begin_frame();
}

void viewport::end_frame()
{
	m_owner_window_->end_frame();
}
