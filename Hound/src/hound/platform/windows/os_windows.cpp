#include "hound/hnd_pch.h"
#include "os_windows.h"

#include "hound/config/engine.h"
#include "hound/platform/windows/display_manager_windows.h"
#include "hound/main/main.h"

windows_os::windows_os()
{
	s_instance_ = this;
}

windows_os::~windows_os()
{
}

bool windows_os::initialize(ref<application> application)
{
	m_application_ = application;
	engine::register_singleton<os, windows_os>();
	engine::register_singleton<display_manager, windows_display_manager>();

	m_display_manager_ = windows_display_manager::get_instance();
	m_display_manager_->initialize(display_manager::get_default_properties(), m_graphics_context_);

	return true;
}

void windows_os::run()
{
	main::start(m_application_);
	main::run();
}

void windows_os::clean_up()
{
	main::clean_up();
}
