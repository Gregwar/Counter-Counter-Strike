#ifndef GO_OS
#define GO_OS

#if defined(linux) || defined(__linux) || defined(__linux__)
	#define GO_LINUX
#endif

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
	#define GO_WINDOWS
#endif

#if !defined(GO_LINUX) && !defined(GO_WINDOWS)
	#error "Your operating system is not GO-Compliant"
#endif

#endif
