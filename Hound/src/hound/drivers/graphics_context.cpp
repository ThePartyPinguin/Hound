#include "hound/hnd_pch.h"
#include "graphics_context.h"
graphics_context* graphics_context::s_instance_ = nullptr;

graphics_context* graphics_context::get_instance()
{
	return s_instance_;
}

graphics_context::graphics_context()
{
	s_instance_ = this;
}
