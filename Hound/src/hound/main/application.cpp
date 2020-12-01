#include "hound/hnd_pch.h"
#include "application.h"

std::string application::s_startup_path_ = "";

const std::string& application::get_startup_path()
{
	return s_startup_path_;
}

application::application()
{
}

application::~application()
{
}
