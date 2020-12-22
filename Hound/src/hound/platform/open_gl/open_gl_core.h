#pragma once
#include <glad/glad.h>

typedef unsigned int gl_object_id;

#define HND_GL_MAJOR_VERSION 4
#define HND_GL_MINOR_VERSION 3
#define HND_GL_PROFILE GLFW_OPENGL_CORE_PROFILE

#if defined(HND_DEBUG) && defined(HND_RENDERER_OPEN_GL)

#include "hound/logging/logger.h"

class hnd_internal_error_checker
{
public:
    template <class result_t, class... gl_args_t, class... args_t>
    static result_t hnd_internal_context_gl_call_gl_func_error_checked(const char* file_name, const char* function_name, const int line_no, result_t(*fun)(gl_args_t...), args_t... args) {
        if constexpr (!std::is_same_v<result_t, void>) {
            auto result = fun(std::forward<args_t>(args)...);

            hnd_internal_context_gl_check_errors(file_name, function_name, line_no);

            return result;
        }
        else {
            fun(std::forward<args_t>(args)...);
            return hnd_internal_context_gl_check_errors(file_name, function_name, line_no);
        }
    }

private:
    static void hnd_internal_context_gl_check_errors(const char* file_name, const char* function_name, const int line_no) {
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {

            const char* error_desc = hnd_internal_context_gl_get_error_description(err);

            logger::log_core_error(file_name, function_name, line_no, "GL::ERROR - ", error_desc);

            throw std::runtime_error(std::string("GL::ERROR - ").append(error_desc));
        }
    }
	
    static const char* hnd_internal_context_gl_get_error_description(GLenum err)
    {
        switch (err) {
        case GL_NO_ERROR:
            return "GL_NO_ERROR: No error has been recorded. The value of this symbolic constant is guaranteed to be 0. ";
        case GL_INVALID_ENUM:
            return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.  ";
        case GL_INVALID_VALUE:
            return "GL_INVALID_VALUE: A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.  ";
        case GL_INVALID_OPERATION:
            return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.  ";
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            return
                "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete."
                "The offending command is ignored and has no other side effect than to set the error flag.";
        case GL_OUT_OF_MEMORY:
            return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded. . ";
        case GL_STACK_UNDERFLOW:
            return "GL_STACK_UNDERFLOW: An attempt has been made to perform an operation that would cause an internal stack to underflow. ";
        case GL_STACK_OVERFLOW:
            return "GL_STACK_OVERFLOW: An attempt has been made to perform an operation that would cause an internal stack to overflow. ";
        default:
            return "No Description";
        }
    }
};

#define HND_GL_CALL(func, ...) hnd_internal_error_checker::hnd_internal_context_gl_call_gl_func_error_checked(__FILE__, __FUNCSIG__, __LINE__, func, __VA_ARGS__)

#else
#define HND_GL_CALL(x, ...) x(__VA_ARGS__)
#endif






// #if HND_DEBUG
// #define HND_GL_CALL(x) x;\
// 		{\
// 			GLenum err;\
// 			while((err = glGetError()) != GL_NO_ERROR)\
// 			{\
// 				char message[128];\
// 				sprintf_s(message, 128, "%s %s:%o", #x, __FILE__, __LINE__);\
// 				HND_CORE_LOG_ERROR("GL::ERROR ", message);\
// 				glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR, 0, GL_DEBUG_SEVERITY_HIGH, 128, message);\
// 				__debugbreak();\
// 			}\
// 		}
// #else
// #define HND_GL_CALL(x) x;
// #endif

