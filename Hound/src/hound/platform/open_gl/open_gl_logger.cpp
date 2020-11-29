#include "hound/hnd_pch.h"
#include "open_gl_logger.h"

void open_gl_logger::init()
{
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(gl_error_callback, 0);
}

void open_gl_logger::gl_error_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	char source_str[64];
	char type_str[64];

	get_gl_error_source(source_str, source);
	get_gl_error_type(type_str, type);

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:
	{
		HND_CORE_LOG_ERROR("GL::HIGH\n\r\tID:", id, "\n\r\tSource: ", source_str, "\n\r\tType: ", type_str, "\n\r\tMessage: ", std::string(message));
		break;
	}
	case GL_DEBUG_SEVERITY_MEDIUM:
	{
		HND_CORE_LOG_WARN("GL::MEDIUM\n\r\tID:", id, "\n\r\tSource: ", source_str, "\n\r\tType: ", type_str, "\n\r\tMessage: ", std::string(message));
		break;
	}
	case GL_DEBUG_SEVERITY_LOW:
	{
		HND_CORE_LOG_INFO("GL::LOW\n\r\tID:", id, "\n\r\tSource: ", source_str, "\n\r\tType: ", type_str, "\n\r\tMessage: ", std::string(message));
		break;
	}
	case GL_DEBUG_SEVERITY_NOTIFICATION:
	{
		return;
		HND_CORE_LOG_TRACE("GL::NOTIFY\n\r\tID:", id, "\n\r\tSource: ", source_str, "\n\r\tType: ", type_str, "\n\r\tMessage: ", std::string(message));
		break;
	}
	default:
		HND_CORE_LOG_ERROR("GL::ERROR\n\r\tID:", id, "\n\r\tSource: ", source_str, "\n\r\tType: ", type_str, "\n\r\tMessage: ", std::string(message));
	}
	//
}

void open_gl_logger::get_gl_error_source(char* log_string, GLenum source)
{
	const char* source_string;
	switch (source)
	{
	case GL_DEBUG_SOURCE_API:
		source_string = "SOURCE_API";
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		source_string = "WINDOW_SYSTEM";
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		source_string = "SHADER_COMPILER";
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		source_string = "THIRD_PARTY";
		break;
	case GL_DEBUG_SOURCE_APPLICATION:
		source_string = "SOURCE_APPLICATION";
		break;
	case GL_DEBUG_SOURCE_OTHER:
		source_string = "UNDEFINED";
		break;
	default:
		source_string = "SOURCE_UNKNOWN";
		break;
	}

	sprintf_s(log_string, 64, "%s(0x%X)", source_string, source);
}

void open_gl_logger::get_gl_error_type(char* log_string, GLenum type)
{
	const char* type_string;
	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:
		type_string = "API_ERROR";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		type_string = "DEPRECATED_BEHAVIOUR";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		type_string = "UNDEFINED_BEHAVIOUR";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		type_string = "PORTABILITY";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		type_string = "PERFORMANCE_ISSUE";
		break;
	case GL_DEBUG_TYPE_MARKER:
		type_string = "MARKER";
		break;
	case GL_DEBUG_TYPE_PUSH_GROUP:
		type_string = "GROUP_PUSHED";
		break;
	case GL_DEBUG_TYPE_POP_GROUP:
		type_string = "GROUP_POPPED";
		break;
	case GL_DEBUG_TYPE_OTHER:
		type_string = "UNDEFINED";
		break;
	default:
		type_string = "SOURCE_UNKNOWN";
		break;
	}

	sprintf_s(log_string, 64, "%s(0x%X)", type_string, type);
}
