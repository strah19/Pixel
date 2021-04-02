#ifndef PLATFORM_DETECTION_H
#define PLATFORM_DETECTION_H

#ifdef _WIN32
	#ifndef _WIN64
		#error "Pixel only supports x64."
	#else
		#define PIXEL_WINDOW_PLATFORM
	#endif
#endif

#endif // !PLATFORM_DETECTION_H
