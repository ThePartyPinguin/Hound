#pragma once
#include "hound/core/input/key_codes.h"

class windows_glfw_helper
{
public:
	static key_code get_key_code(int key);
	static key_code get_mouse_button(int button);
	static key_action get_key_action(int action);
};