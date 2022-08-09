#ifndef fcaseopen_h
#define fcaseopen_h

#include <stdio.h>

#if defined(__cplusplus)
extern "C" {
#endif

	extern FILE* fcaseopen(char const* path, char const* mode);

	extern void casechdir(char const* path);
#if !defined(_WIN32)
	extern int casepath(char const* path, char* r);
#endif

#if defined(__cplusplus)
}
#endif

#endif