#pragma once

#ifdef HND_ENABLE_ASSERT

	#define HND_CORE_ASSERT(x, ...) if(!(x)){HND_LOG_ERROR(__VA_ARGS__);  __debugbreak();}

#else

	#define HND_CORE_ASSERT(x, ...)
#endif
