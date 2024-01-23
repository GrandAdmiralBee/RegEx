#include "limits.h"

#ifndef FCP_DEFS_H__
#define FCP_DEFS_H__

#if defined WIN32 || defined WIN64
 // disable warning about deprecated function
 #pragma warning(disable : 4996)
 // disable warning about inconsistent dll linkage
 #pragma warning(disable : 4273)
#endif

#if defined WIN32 || defined WIN64
 // force time_t to be 32 bit
 //#define _USE_64BIT_TIME_T 
#endif

// provide definition of keyword required to build DLL
#ifndef DLLEXPORT
#if defined WIN32 || defined WIN64
  #define DLLEXPORT __declspec( dllexport )
  #pragma warning(disable : 4251)
 #else
  #define DLLEXPORT
 #endif
#endif

#ifndef FCP_PI
	#define FCP_PI  3.1415926535897932385
#endif

#ifndef FCP_NO_NAME
	#define FCP_NO_NAME  "---"
#endif

#ifndef FCP_LOCALE
	#define LOC_WIN   0
	#define LOC_UNIX  1
#if defined WIN32 || defined WIN64
	#define FCP_LOCALE LOC_WIN
	//#define PATH_SEPARATOR '\\'
	#define PATH_SEPARATOR '/'
	#define DIRS_SEPARATOR ';'
 #else
	#define FCP_LOCALE LOC_UNIX
	#define PATH_SEPARATOR '/'
	#define DIRS_SEPARATOR ':'
 #endif
#endif

# ifndef  DELETE
# define  DELETE( Pointer ) \
  if (Pointer) \
  { delete Pointer; Pointer = 0; }
# endif

# ifndef  DELETE_ARRAY
# define  DELETE_ARRAY( Array ) \
  if (Array) \
  { delete [] Array; Array = 0; }
# endif

typedef unsigned int uint;

#endif /* FCP_DEFS_H__ */
