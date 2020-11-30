#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#include "hound/core/object/object.h"

class logger
{
public:
	static void init();

	template<typename... T_Args>
	static void log_core_trace(const char* file_name, const char* function_name, const int line_no, const T_Args&... log_args);
	template<typename... T_Args>
	static void log_core_info(const char* file_name, const char* function_name, const int line_no, const T_Args&...log_args);
	template<typename... T_Args>
	static void log_core_warn(const char* file_name, const char* function_name, const int line_no, const T_Args&...log_args);
	template<typename... T_Args>
	static void log_core_error(const char* file_name, const char* function_name, const int line_no, const T_Args&...log_args);


	template<typename... T_Args>
	static void log_trace(const char* file_name, const char* function_name, const int line_no, const T_Args&...log_args);
	template<typename... T_Args>
	static void log_info(const char* file_name, const char* function_name, const int line_no, const T_Args&...log_args);
	template<typename... T_Args>
	static void log_warn(const char* file_name, const char* function_name, const int line_no, const T_Args&...log_args);
	template<typename... T_Args>
	static void log_error(const char* file_name, const char* function_name, const int line_no, const T_Args&...log_args);

private:
	static std::string format_file_info_string(const char* file_name, const char* function_name, const int line_no, const int arg_count);

	static std::shared_ptr<spdlog::logger>& get_core_logger();
	static std::shared_ptr<spdlog::logger>& get_application_logger();
	static std::shared_ptr<spdlog::logger> s_core_logger_;
	static std::shared_ptr<spdlog::logger> s_application_logger_;
};

template <typename... T_Args>
void logger::log_core_trace(const char* file_name, const char* function_name, const int line_no, const T_Args&...log_args)
{
	get_core_logger()->trace(format_file_info_string(file_name, function_name, line_no, sizeof...(T_Args)), log_args...);
}

template <typename... T_Args>
void logger::log_core_info(const char* file_name, const char* function_name, const int line_no, const T_Args&...log_args)
{
	get_core_logger()->info(format_file_info_string(file_name, function_name, line_no, sizeof...(T_Args)), log_args...);
}

template <typename... T_Args>
void logger::log_core_warn(const char* file_name, const char* function_name, const int line_no, const T_Args&...log_args)
{
	get_core_logger()->warn(format_file_info_string(file_name, function_name, line_no, sizeof...(T_Args)), log_args...);
}

template <typename... T_Args>
void logger::log_core_error(const char* file_name, const char* function_name, const int line_no, const T_Args&...log_args)
{
	get_core_logger()->error(format_file_info_string(file_name, function_name, line_no, sizeof...(T_Args)), log_args...);
}

template <typename... T_Args>
void logger::log_trace(const char* file_name, const char* function_name, const int line_no, const T_Args&...log_args)
{
	get_application_logger()->trace(format_file_info_string(file_name, function_name, line_no, sizeof...(T_Args)), log_args...);
}

template <typename... T_Args>
void logger::log_info(const char* file_name, const char* function_name, const int line_no, const T_Args&...log_args)
{
	get_application_logger()->info(format_file_info_string(file_name, function_name, line_no, sizeof...(T_Args)), log_args...);
}

template <typename... T_Args>
void logger::log_warn(const char* file_name, const char* function_name, const int line_no, const T_Args&...log_args)
{
	get_application_logger()->warn(format_file_info_string(file_name, function_name, line_no, sizeof...(T_Args)), log_args...);
}

template <typename... T_Args>
void logger::log_error(const char* file_name, const char* function_name, const int line_no, const T_Args&...log_args)
{
	get_application_logger()->error(format_file_info_string(file_name, function_name, line_no, sizeof...(T_Args)), log_args...);
}

// #define HND_CORE_LOG_TRACE(...)		::hound::logging::logger::get_core_logger()->trace(__VA_ARGS__)
#define HND_CORE_LOG_TRACE(...)		logger::log_core_trace(__FILE__, __FUNCSIG__, __LINE__, __VA_ARGS__)
#define HND_CORE_LOG_INFO(...)		logger::log_core_info(__FILE__, __FUNCSIG__, __LINE__, __VA_ARGS__)
#define HND_CORE_LOG_WARN(...)		logger::log_core_warn(__FILE__, __FUNCSIG__, __LINE__, __VA_ARGS__)
#define HND_CORE_LOG_ERROR(...)		logger::log_core_error(__FILE__, __FUNCSIG__, __LINE__, __VA_ARGS__)

#define HND_LOG_TRACE(...)			logger::log_trace(__FILE__, __FUNCSIG__, __LINE__, __VA_ARGS__)
#define HND_LOG_INFO(...)			logger::log_info(__FILE__, __FUNCSIG__, __LINE__, __VA_ARGS__)
#define HND_LOG_WARN(...)			logger::log_warn(__FILE__, __FUNCSIG__, __LINE__, __VA_ARGS__)
#define HND_LOG_ERROR(...)			logger::log_error(__FILE__, __FUNCSIG__, __LINE__, __VA_ARGS__)

