#pragma once


#define HND_PLATFORM_UNKNOWN platform::unknown
#define HND_PLATFORM_WINDOWS_64 platform::windows_x64
#define HND_PLATFORM_WINDOWS_32 platform::windows_x32
#define HND_PLATFORM_LINUX platform::linux
#define HND_PLATFORM_OSX platform::osx
#define HND_PLATFORM_IOS platform::ios
#define HND_PLATFORM_ANDROID platform::android



#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
//define something for Windows (32-bit and 64-bit, this part is common)
	#ifdef _WIN64
		#define HND_PLATFORM HND_PLATFORM_WINDOWS_64
	#else
		#define HND_PLATFORM HND_PLATFORM_WINDOWS_32
	#endif
#elif __APPLE__
	#include <TargetConditionals.h>
	#if TARGET_IPHONE_SIMULATOR
		#define HND_PLATFORM HND_PLATFORM_UNKNOWN
	#elif TARGET_OS_IPHONE
		#define HND_PLATFORM HND_PLATFORM_IOS
	#elif TARGET_OS_MAC
		#define HND_PLATFORM HND_PLATFORM_OSX
	#else
		#define HND_PLATFORM HND_PLATFORM_UNKNOWN
	#endif
#elif __linux__
	#define HND_PLATFORM HND_PLATFORM_LINUX
#else
	#define HND_PLATFORM HND_PLATFORM_UNKNOWN
#endif



//
// class platform_identifier : public singleton<platform_identifier>
// {
// public:
// 	static platform get_platform();
// };

