#include "hound/hnd_pch.h"
#include "os_windows.h"

#include "hound/config/engine.h"
#include "hound/main/main.h"
#include "hound/platform/glfw/glfw_display_driver.h"
#include "hound/platform/windows/io/file/windows_file_handle.h"

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

file_handle* windows_os::file_handle_from_asset_path(const char* path)
{
	const std::string full_path = convert_asset_path_to_absolute(path);
	return file_handle_from_absolute_path(full_path.c_str());
}

file_handle* windows_os::file_handle_from_absolute_path(const char* path)
{
	std::string final_path = std::string(path);
	fix_file_path(final_path);
	windows_file_handle* handler = object_database::create_object_instance<windows_file_handle>();
	handler->m_file_path_ = final_path;
	handler->init();
	return handler;
}

void windows_os::fix_file_path(std::string& path)
{
	size_t start_pos = 0;
	while ((start_pos = path.find('/', start_pos)) != std::string::npos) {
		path.replace(start_pos, 1, 1, '\\');
		start_pos += 1;
	}
}

std::string windows_os::convert_asset_path_to_absolute(const char* asset_path)
{
	return application::get_startup_path() + "/Assets/" + asset_path;
}
