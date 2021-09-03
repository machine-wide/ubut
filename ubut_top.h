#ifndef UBUT_TOP_INC
#define UBUT_TOP_INC

#if defined(__clang__)
#pragma clang system_header
#endif // __clang__

/*
-------------------------------------------------------------------------------
nicked from <yvals_core.h>
this is used in the MS STL source whenever they use what they do
advise customers not to use ;)
*/

// clang-format off
#ifndef _STL_DISABLE_DEPRECATED_WARNING
#ifdef __clang__
#define _STL_DISABLE_DEPRECATED_WARNING \
	_Pragma("clang diagnostic push")    \
	_Pragma("clang diagnostic ignored \"-Wdeprecated-declarations\"")
#else // __clang__
#define _STL_DISABLE_DEPRECATED_WARNING \
	__pragma(warning(push))             \
	__pragma(warning(disable : 4996)) // was declared deprecated
#endif // __clang__
#endif // _STL_DISABLE_DEPRECATED_WARNING
// clang-format on

#ifndef _STL_RESTORE_DEPRECATED_WARNING
#ifdef __clang__
#define _STL_RESTORE_DEPRECATED_WARNING _Pragma("clang diagnostic pop")
#else // __clang__
#define _STL_RESTORE_DEPRECATED_WARNING __pragma(warning(pop))
#endif // __clang__
#endif // _STL_RESTORE_DEPRECATED_WARNING

// instead of including the whole <atomic> because of line 32
#ifndef _Compiler_barrier
#define _Compiler_barrier() _STL_DISABLE_DEPRECATED_WARNING _ReadWriteBarrier() _STL_RESTORE_DEPRECATED_WARNING
#endif
/*
official advice is not to use _ReadWriteBarrier, but then there is a lot of _Compiler_barrier() all arround the
<atomic> et all in the MS STL code
*/

/*
-------------------------------------------------------------------------------
set the WINVER and _WIN32_WINNT macros to the oldest supported platform
#include <winsdkver.h>
*/

#undef WINVER
#define WINVER 0x0A00

#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00

#define NOMINMAX
#define STRICT 1
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
// #include <processenv.h>

/*
-------------------------------------------------------------------------------
*/
#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif

#include <crtdbg.h>
#include <errno.h>
#include <stdbool.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <inttypes.h>

/*
	io.h contains definitions for some structures with natural padding. This is
	uninteresting, but for some reason MSVC's behaviour is to warn about
	including this system header. That *is* interesting
*/
#pragma warning(disable : 4820)
#pragma warning(push, 1)
#include <io.h>
#pragma warning(pop)
/*
-------------------------------------------------------------------------------
UBUT stuff begins here
-------------------------------------------------------------------------------
*/

// clang-cl.exe has them both defined
// thus it is not enugh to use _MSC_VER only
#ifdef _WIN32
#define UBUT_IS_WIN
#else
/* but then ... */
#error This is WIN32 only, for OS agnostic experience please use sheredom/ubench from GitHub
#endif

/*
clang_cl.exe is LLVM c++ compiler compiled by MSFT and included
with VS 2019
*/
#undef UBUT_IS_CLANG_CL
#ifdef _MSC_VER
#ifdef __clang__
#define UBUT_IS_CLANG_CL 1
#endif // __clang__
#endif // _MSC_VER

#ifdef __clang__
#define UBUT_UNUSED __attribute__((unused))
#else
#define UBUT_UNUSED
#endif

/*
   Disable warning about not inlining 'inline' functions.
   TODO: We'll fix this later by not using fprintf within our macros, and
   instead use snprintf to a realloc'ed buffer.
*/
#pragma warning(disable : 4710)

/*
   Disable warning about inlining functions that are not marked 'inline'.
   TODO: add a UBUT_NOINLINE onto the macro generated functions to fix this.
*/
#pragma warning(disable : 4711)
#pragma warning(push, 1)

typedef __int64 ubut_int64_t;
typedef unsigned __int64 ubut_uint64_t;

#pragma warning(pop)

#if defined(__cplusplus)
#define UBUT_C_FUNC extern "C"
#else
#define UBUT_C_FUNC
#endif

#if defined(__cplusplus) && (__cplusplus >= 201103L)
#define UBUT_NOEXCEPT noexcept
#else
#define UBUT_NOEXCEPT
#endif

#ifdef UBUT_IS_CLANG_CL
#define UBUT_NOTHROW __declspec(nothrow)
#else
#define UBUT_NOTHROW
#endif

#define UBUT_PRId64 "I64d"
#define UBUT_PRIu64 "I64u"
// #define UBUT_INLINE __forceinline
#define UBUT_NOINLINE __declspec(noinline)
#define UBUT_FORCEINLINE __forceinline

#if defined(_WIN64)
#define UBUT_SYMBOL_PREFIX
#else
#define UBUT_SYMBOL_PREFIX "_"
#endif

#pragma section(".CRT$XCU", read)
#define UBUT_INITIALIZER(f)                                                   \
	static void __cdecl f(void);                                              \
	__pragma(comment(linker, "/include:" UBUT_SYMBOL_PREFIX #f "_"));         \
	UBUT_C_FUNC __declspec(allocate(".CRT$XCU")) void(__cdecl * f##_)(void) = \
		f;                                                                    \
	static void __cdecl f(void)

// clang on win aka clang-cl.exe
#ifdef __clang__
#undef UBUT_INITIALIZER
#define UBUT_INITIALIZER(f)                           \
	static void f(void) __attribute__((constructor)); \
	static void f(void)
#endif // __clang__

#define __STDC_FORMAT_MACROS 1

#if defined(__clang__) && defined(__has_warning)
#if __has_warning("-Wreserved-id-macro")
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreserved-id-macro"
#pragma clang diagnostic pop
#endif
#endif // clang has warning

#if defined(__cplusplus)
#define UBUT_CAST(type, x) static_cast<type>(x)
#define UBUT_PTR_CAST(type, x) reinterpret_cast<type>(x)
#define UBUT_EXTERN extern "C"
#define UBUT_NULL NULL
#else
#define UBUT_CAST(type, x) ((type)x)
#define UBUT_PTR_CAST(type, x) ((type)x)
#define UBUT_EXTERN extern
#define UBUT_NULL 0
#endif

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wvariadic-macros"
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#endif

#ifdef __clang__
#pragma clang diagnostic pop
#endif

//#ifdef UBUT_IS_WIN
#define UBUT_SNPRINTF(BUFFER, N, ...) _snprintf_s(BUFFER, N, N, __VA_ARGS__)
//#else
//#ifdef __clang__
//#pragma clang diagnostic push
//#pragma clang diagnostic ignored "-Wvariadic-macros"
//#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
//#endif
//#define UBUT_SNPRINTF(...) snprintf(__VA_ARGS__)
//#ifdef __clang__
//#pragma clang diagnostic pop
//#endif
//#endif

#ifdef __cplusplus
#define UBUT_BEGIN_EXTERN_C extern "C" {
#else
#define UBUT_BEGIN_EXTERN_C 
#endif

#ifdef __cplusplus    
#define UBUT_END_EXTERN_C }
#else
#define UBUT_END_EXTERN_C 
#endif

UBUT_BEGIN_EXTERN_C

UBUT_FORCEINLINE int ubut_strncmp(const char *a, const char *b, size_t n)
	{
		/* strncmp breaks on Wall / Werror on gcc/clang, so we avoid using it */
		unsigned i;

		for (i = 0; i < n; i++)
		{
			if (a[i] < b[i])
			{
				return -1;
			}
			else if (a[i] > b[i])
			{
				return 1;
			}
		}

		return 0;
	}

UBUT_FORCEINLINE FILE *ubut_fopen(const char *filename, const char *mode)
	{
#ifdef UBUT_IS_WIN
		FILE *file;
		if (0 == fopen_s(&file, filename, mode))
		{
			return file;
		}
		else
		{
			return 0;
		}
#else
		return fopen(filename, mode);
#endif
	}

	/*
CAUTION: this is WIN32 only version that replaces 
utest_ns() and ubut_ns() from original OS agnostic
separate headers utest.h and ubench.h
*/
UBUT_FORCEINLINE ubut_int64_t ubut_ns(void)
	{
		//#ifdef UBENCH_IS_WIN
		LARGE_INTEGER counter;
		LARGE_INTEGER frequency;
		QueryPerformanceCounter(&counter);
		QueryPerformanceFrequency(&frequency);
		return UBUT_CAST(ubut_int64_t,
						 (counter.QuadPart * 1000000000) / frequency.QuadPart);
	}

UBUT_END_EXTERN_C
	/*
-------------------------------------------------------------------------------
CAUTION: This is compile time. You must define  _WIN32_WINNT in project settings.
(and WINVER too)

Meaning: you compile and run on W10 and all is fine.
But, you run the same exe on W7, and squigly bits will be shown instead of colours

If you check at runtime and stop if running bellow W10, in that respect you will be ok.
-------------------------------------------------------------------------------
*/

#if (_WIN32_WINNT == 0x0A00)
#define UBUT_VT_ESC "\x1b["
#define UBUT_VT_RESET UBUT_VT_ESC "0m"
#define UBUT_VT_GRAY UBUT_VT_ESC "90m"
#define UBUT_VT_BLUE UBUT_VT_ESC "94m"
#define UBUT_VT_CYAN UBUT_VT_ESC "36m"
#define UBUT_VT_YELLOW UBUT_VT_ESC "33m"
#define UBUT_VT_GREEN UBUT_VT_ESC "32m"
#define UBUT_VT_RED UBUT_VT_ESC "31m"
#define UBUT_VT_MAGENTA UBUT_VT_ESC "35m"
#else // ! WIN10
#define UBUT_VT_ESC ""
#define UBUT_VT_RESET UBUT_VT_ESC ""
#define UBUT_VT_GRAY UBUT_VT_ESC ""
#define UBUT_VT_BLUE UBUT_VT_ESC ""
#define UBUT_VT_CYAN UBUT_VT_ESC ""
#define UBUT_VT_YELLOW UBUT_VT_ESC ""
#define UBUT_VT_GREEN UBUT_VT_ESC ""
#define UBUT_VT_RED UBUT_VT_ESC ""
#define UBUT_VT_MAGENTA UBUT_VT_ESC ""
#endif


// #define DBJ_UBUT_SIMPLE_LOG // THE BIG ISSUE

#ifdef DBJ_UBUT_SIMPLE_LOG
#define DBJ_SIMPLELOG_USER_DEFINED_MACRO_NAMES
#include "../simplelog/dbj_simple_log.h"

#undef UBUT_TRACE
#undef UBUT_DEBUG
#undef UBUT_INFO
#undef UBUT_WARN
#undef UBUT_ERROR
#undef UBUT_FATAL

#define UBUT_TRACE(...) dbj_log_trace(__VA_ARGS__)
#define UBUT_DEBUG(...) dbj_log_debug(__VA_ARGS__)
#define UBUT_INFO(...) dbj_log_info(__VA_ARGS__)
#define UBUT_WARN(...) dbj_log_warn(__VA_ARGS__)
#define UBUT_ERROR(...) dbj_log_error(__VA_ARGS__)
#define UBUT_FATAL(...) dbj_log_fatal(__VA_ARGS__)

#else
// If not using simple log, what are you using?
#define UBUT_TRACE(...) fprintf(stderr, UBUT_VT_RESET "\n" UBUT_VT_GRAY), fprintf(stderr, __VA_ARGS__)
#define UBUT_DEBUG(...) fprintf(stderr, UBUT_VT_RESET "\n" UBUT_VT_GREEN), fprintf(stderr, __VA_ARGS__)
#define UBUT_INFO(...) fprintf(stderr, UBUT_VT_RESET "\n" UBUT_VT_RESET), fprintf(stderr, __VA_ARGS__)
#define UBUT_WARN(...) fprintf(stderr, UBUT_VT_RESET "\n" UBUT_VT_YELLOW), fprintf(stderr, __VA_ARGS__)
#define UBUT_ERROR(...) fprintf(stderr, UBUT_VT_RESET "\n" UBUT_VT_RED), fprintf(stderr, __VA_ARGS__)
#define UBUT_FATAL(...) fprintf(stderr, UBUT_VT_RESET "\n" UBUT_VT_RED), fprintf(stderr, __VA_ARGS__)

#endif // ! DBJ_UBUT_SIMPLE_LOG

#define UBUT_CONSOLE_COLOR_RESET(...)   \
	do                                  \
	{                                   \
		fprintf(stdout, UBUT_VT_RESET); \
		fprintf(stderr, UBUT_VT_RESET); \
		fflush(stdout);                 \
		fflush(stderr);                 \
	} while (0)

/*
-------------------------------------------------------------------------------
EOF
-------------------------------------------------------------------------------
*/
#endif // UBUT_TOP_INC
