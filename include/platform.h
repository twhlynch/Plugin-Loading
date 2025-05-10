#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef _WIN32
	#define LIBRARY_EXTENSION "dll"
#elif defined(__APPLE__)
	#define LIBRARY_EXTENSION "dylib"
#else
	#define LIBRARY_EXTENSION "so"
#endif

#endif // PLATFORM_H