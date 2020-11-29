#include "hound/hnd_pch.h"
#include "os.h"

#include "hound/config/engine.h"
#include "hound/core/scene/render_target/window/window.h"
os* os::s_instance_ = nullptr;

platform os::get_platform()
{
	HND_CORE_LOG_ERROR("Could not get platform!");
	return platform::unknown;
}

os::os()
{
	m_application_ = nullptr;
	m_graphics_context_ = nullptr;
	m_display_manager_ = nullptr;
	m_main_window_ = nullptr;
}

os::~os()
{
}

bool os::initialize(ref<application> application)
{
	HND_CORE_LOG_ERROR("Could not initialize unkown platform!");
	return false;
}

void os::run()
{
	HND_CORE_LOG_ERROR("Could not run unkown platform!");
}

void os::clean_up()
{
	HND_CORE_LOG_ERROR("Could not run unkown platform!");
}

void os::set_main_window(window* window)
{
	m_main_window_ = window;
	m_main_window_->subscribe<window_close_event>(this);
}

void os::on_event(const window_close_event&)
{
	HND_LOG_TRACE("Window close event caputered by os!");
}

void os::on_event(const input_event_key& e)
{
	HND_LOG_TRACE("Key input! - ", e.get_key(), "\r\n\tHas modifer:", (e.get_modifier_keys() & 0));
}

void os::on_event(const input_event_mouse& e)
{
}

void os::on_event(const input_event_mouse_move& e)
{
}
