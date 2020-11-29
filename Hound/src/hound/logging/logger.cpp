#include "hound/hnd_pch.h"

#include "logger.h"
#include <spdlog/sinks/stdout_color_sinks.h>

std::shared_ptr<spdlog::logger> logger::s_core_logger_;
std::shared_ptr<spdlog::logger> logger::s_application_logger_;

void logger::init()
{
	spdlog::set_pattern("%^[%l][%T] %n Thread:%t\n%v%$");

	s_core_logger_ = spdlog::stderr_color_mt("HOUND_CORE");
	s_core_logger_->set_level(spdlog::level::trace);

	s_application_logger_ = spdlog::stderr_color_mt("HOUND_APP");
	s_application_logger_->set_level(spdlog::level::trace);
}

std::string logger::format_file_info_string(const char* file_name, const char* function_name, const int line_no, const int arg_count)
{
	std::stringstream ss;

	ss << file_name << ':' << line_no << " - " << function_name << "\n\n";

	for (int i = 0; i < arg_count; ++i)
	{
		ss << "{}";
	}

	ss << '\n';

	return ss.str();
}

std::shared_ptr<spdlog::logger>& logger::get_core_logger()
{
	return s_core_logger_;
}

std::shared_ptr<spdlog::logger>& logger::get_application_logger()
{
	return s_application_logger_;
}