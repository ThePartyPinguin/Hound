#include "hound/hnd_pch.h"
#include "camera_manager.h"

camera_manager* camera_manager::s_instance_ = nullptr;

camera_manager* camera_manager::get_singleton()
{
	return s_instance_;
}

ref<camera> camera_manager::create_camera()
{
	return create_ref<camera>();
}

camera_manager::camera_manager() : object()
{
	s_instance_ = this;
}
