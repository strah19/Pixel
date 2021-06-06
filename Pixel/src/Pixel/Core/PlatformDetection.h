#ifndef PLATFORM_DETECTION_H
#define PLATFORM_DETECTION_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	#define PIXEL_WINDOW_PLATFORM
#ifdef _WIN64
	#define PIXEL_WINDOW_PLATFORM
#else
	#error "Pixel only supports x64."
#endif
#elif __APPLE__
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR
	#error "Pixel does not support Iphone Simulators."
#elif TARGET_OS_IPHONE
	#error "Pixel does not support IOS Devices."
#elif TARGET_OS_MAC
	#error "Pixel does not support Mac Devices."
#else
	#error "Unknown Apple platform."
#endif
#elif __linux__
	#error "Pixel does not support Linux Devices."
#elif __unix__ 
	#error "Pixel does not support Unix Devices."
#elif defined(_POSIX_VERSION)
	#error "Pixel does not support Posix Versions."
#else
#   error "Unknown compiler!"
#endif


#endif // !PLATFORM_DETECTION_H
