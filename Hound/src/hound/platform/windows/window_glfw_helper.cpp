#include "hound/hnd_pch.h"
#include "window_glfw_helper.h"

#include "GLFW/glfw3.h"

//https://www.glfw.org/docs/3.3.2/group__keys.html#gac556b360f7f6fca4b70ba0aecf313fd4
key_code windows_glfw_helper::get_key_code(int key)
{
	//Return characters a-z
	if(key >= 65 && key <= 90)
	{
		const int offset = static_cast<int>(key_code::A);
		return static_cast<key_code>((key - 65) + offset);
	}

	//return numbers 1-0
	if(key >= 48 && key <= 57)
	{
		const int offset = static_cast<int>(key_code::_0);
		return static_cast<key_code>((key - 48) + offset);
	}

	//Return F-keys F1-F25
	if(key >= 290 && key <= 314)
	{
		const int offset = static_cast<int>(key_code::F_1);
		return static_cast<key_code>((key - 290) + offset);
	}

	//Return numpad_numbers 0-9
	if (key >= 320 && key <= 329)
	{
		const int offset = static_cast<int>(key_code::NUM_0);
		return static_cast<key_code>((key - 320) + offset);
	}

	//Switch case for special characters, space-bar, enter, escape etc.
	//Default case return UNKNOWN
	switch (key)
	{
	case GLFW_KEY_SPACE:
		return key_code::S_SPACE;
	case GLFW_KEY_APOSTROPHE:
		return key_code::S_CHAR_APOSTROPHE;
	case GLFW_KEY_COMMA:
		return key_code::S_CHAR_COMMA;
	case GLFW_KEY_MINUS:
		return key_code::S_CHAR_MINUS;
	case GLFW_KEY_PERIOD:
		return key_code::S_CHAR_PERIOD;
	case GLFW_KEY_SLASH:
		return key_code::S_CHAR_SLASH;
	case GLFW_KEY_SEMICOLON:
		return key_code::S_CHAR_SEMI_COLON;
	case GLFW_KEY_EQUAL:
		return key_code::S_CHAR_EQUALS;
	case GLFW_KEY_LEFT_BRACKET:
		return key_code::S_CHAR_LEFT_BRACKET;
	case GLFW_KEY_BACKSLASH:
		return key_code::S_CHAR_BACK_SLASH;
	case GLFW_KEY_RIGHT_BRACKET:
		return key_code::S_CHAR_RIGHT_BRACKET;
	case GLFW_KEY_GRAVE_ACCENT:
		return key_code::S_CHAR_GRAVE_ACCENT;
	case GLFW_KEY_ESCAPE:
		return key_code::S_ESCAPE;
	case GLFW_KEY_ENTER:
		return key_code::S_ENTER;
	case GLFW_KEY_TAB:
		return key_code::S_TAB;
	case GLFW_KEY_BACKSPACE:
		return key_code::S_BACK_SPACE;
	case GLFW_KEY_INSERT:
		return key_code::S_INSERT;
	case GLFW_KEY_DELETE:
		return key_code::S_DELETE;
	case GLFW_KEY_RIGHT:
		return key_code::S_ARROW_RIGHT;
	case GLFW_KEY_LEFT:
		return key_code::S_ARROW_LEFT;
	case GLFW_KEY_DOWN:
		return key_code::S_ARROW_DOWN;
	case GLFW_KEY_UP:
		return key_code::S_ARROW_UP;
	case GLFW_KEY_PAGE_UP:
		return key_code::S_PAGE_UP;
	case GLFW_KEY_PAGE_DOWN:
		return key_code::S_PAGE_DOWN;
	case GLFW_KEY_HOME:
		return key_code::S_HOME;
	case GLFW_KEY_END:
		return key_code::S_END;
	case GLFW_KEY_CAPS_LOCK:
		return key_code::S_CAPS_LOCK;
	case GLFW_KEY_SCROLL_LOCK:
		return key_code::S_SCROLL_LOCK;
	case GLFW_KEY_NUM_LOCK:
		return key_code::S_NUM_LOCK;
	case GLFW_KEY_PRINT_SCREEN:
		return key_code::S_PRINT_SCREEN;
	case GLFW_KEY_PAUSE:
		return key_code::S_PAUSE_BREAK;
	case GLFW_KEY_KP_DECIMAL:
		return key_code::S_KP_DECIMAL;
	case GLFW_KEY_KP_DIVIDE:
		return key_code::S_KP_DIVIDE;
	case GLFW_KEY_KP_MULTIPLY:
		return key_code::S_KP_MULTIPLY;
	case GLFW_KEY_KP_SUBTRACT:
		return key_code::S_KP_SUBTRACT;
	case GLFW_KEY_KP_ADD:
		return key_code::S_KP_ADD;
	case GLFW_KEY_KP_ENTER:
		return key_code::S_KP_ENTER;
	case GLFW_KEY_KP_EQUAL:
		return key_code::S_KP_ENTER;
	case GLFW_KEY_LEFT_SHIFT:
		return key_code::S_LEFT_SHIFT;
	case GLFW_KEY_LEFT_CONTROL:
		return key_code::S_LEFT_CONTROL;
	case GLFW_KEY_LEFT_ALT:
		return key_code::S_LEFT_ALT;
	case GLFW_KEY_LEFT_SUPER:
		return key_code::S_LEFT_SUPER;
	case GLFW_KEY_RIGHT_SHIFT:
		return key_code::S_RIGHT_SHIFT;
	case GLFW_KEY_RIGHT_CONTROL:
		return key_code::S_RIGHT_CONTROL;
	case GLFW_KEY_RIGHT_ALT:
		return key_code::S_RIGHT_ALT;
	case GLFW_KEY_RIGHT_SUPER:
		return key_code::S_RIGHT_SUPER;
	case GLFW_KEY_MENU:
		return key_code::S_MENU;
	default:
		return key_code::UNKNOWN;
	}
}

key_code windows_glfw_helper::get_mouse_button(int button)
{
	if(button >= 0 && button <= 7)
	{
		const int offset = static_cast<int>(key_code::MOUSE_1);
		return static_cast<key_code>(button + offset);
	}

	return key_code::UNKNOWN;
}

key_action windows_glfw_helper::get_key_action(int action)
{
	switch (action)
	{
	case GLFW_RELEASE:
		return key_action::RELEASED;
	case GLFW_PRESS:
		return key_action::PRESSED;
	case GLFW_REPEAT:
		return key_action::REPEAT;
	default:
		return key_action::UNKNOWN;
	}
}
