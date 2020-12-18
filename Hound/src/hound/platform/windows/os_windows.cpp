#include "hound/hnd_pch.h"
#include "os_windows.h"

#include "hound/config/engine.h"
#include "hound/main/main.h"
#include "hound/platform/glfw/glfw_display_driver.h"

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
	display_driver* driver = engine::register_singleton<display_driver, glfw_display_driver>();
	driver->init();
	
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
