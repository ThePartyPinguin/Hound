#pragma once
#include "hound/core/object/object.h"
#include "hound/core/scene/camera/camera.h"

class camera_manager : public object
{
public:
	static camera_manager* get_singleton();
	
	ref<camera> create_camera();

	camera_manager();
	
private:
	static camera_manager* s_instance_;
};

