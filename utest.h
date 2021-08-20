/*
(C) 2020 by dbj@dbj.org -- LICENSE_DBJ -- https://dbj.org/license_dbj
*/
/*
   The latest version of this library is available on GitHub;
   https://github.com/sheredom/utest.h

   For more information, please refer to <http://unlicense.org/>


   #define UTEST_IMPLEMENTATION  in only one compilation unit
*/

#ifndef SHEREDOM_UTEST_H_INCLUDED
#define SHEREDOM_UTEST_H_INCLUDED

#include "ubut_top.h"
// #include "ubut_print.h"

// clang-cl.exe has them both defined
// _MSC_VER and __clang__
// ditto ...
#ifdef _WIN32 
#define UTEST_IS_WIN 
#endif

// not in use
#ifdef UTEST_IS_WIN 
#ifdef __clang__
#define UTEST_IS_WIN_CLANG
#endif
#endif

UBUT_BEGIN_EXTERN_C

typedef void (*utest_testcase_t)(int*, size_t);

struct utest_test_state_s {
	utest_testcase_t func;
	size_t test_index;
	char* name;
};

// static struct utest_test_state_s empty_utest_test_state_s = { NULL, 0, NULL };

struct utest_state_s {
	struct utest_test_state_s* tests;
	size_t tests_length;
	FILE* output;
};

/* extern to the global state utest needs to execute */
 extern struct utest_state_s utest_state;

#define UTEST_XML_OUT(...)    do {                                            \
  if (utest_state.output) {                                                   \
	fprintf(utest_state.output, __VA_ARGS__);                                 \
  }   \
} while(0)

#define UTEST_REZ_OUT(...)    do {                                            \
  if (utest_state.output) {                                                   \
	fprintf(utest_state.output, __VA_ARGS__);                                 \
  }   \
UBUT_INFO( __VA_ARGS__ ) ; \
} while(0)

 UBUT_END_EXTERN_C

#define UTEST_HAVING_TYPE_PRINTERS

#ifdef UTEST_HAVING_TYPE_PRINTERS

#ifdef __clang__
#ifdef __cplusplus
#define UBUT_OVERLOADABLE 
#else
#define UBUT_OVERLOADABLE __attribute__((overloadable))
#endif // C++
#endif // clang

UBUT_FORCEINLINE UBUT_OVERLOADABLE void utest_type_printer(float f) {
	UTEST_REZ_OUT("%f", UBUT_CAST(double, f));
}

UBUT_FORCEINLINE UBUT_OVERLOADABLE void utest_type_printer(double d) {
	UTEST_REZ_OUT("%f", d);
}

UBUT_FORCEINLINE UBUT_OVERLOADABLE void utest_type_printer(long double d) {
	UTEST_REZ_OUT("%Lf", d);
}

UBUT_FORCEINLINE UBUT_OVERLOADABLE void utest_type_printer(int i) {
	UTEST_REZ_OUT("%d", i);
}

UBUT_FORCEINLINE UBUT_OVERLOADABLE void utest_type_printer(unsigned int i) {
	UTEST_REZ_OUT("%u", i);
}

UBUT_FORCEINLINE UBUT_OVERLOADABLE void utest_type_printer(long int i) {
	UTEST_REZ_OUT("%ld", i);
}

UBUT_FORCEINLINE UBUT_OVERLOADABLE void utest_type_printer(long unsigned int i) {
	UTEST_REZ_OUT("%lu", i);
}

UBUT_FORCEINLINE UBUT_OVERLOADABLE void utest_type_printer(const void* p) {
	UTEST_REZ_OUT("%p", p);
}

/*
   long long is a c++11 extension
*/
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) ||              \
	defined(__cplusplus) && (__cplusplus >= 201103L)

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#endif

UBUT_FORCEINLINE UBUT_OVERLOADABLE void utest_type_printer(long long int i);
UBUT_FORCEINLINE UBUT_OVERLOADABLE void utest_type_printer(long long int i) {
	UTEST_REZ_OUT("%lld", i);
}

UBUT_FORCEINLINE UBUT_OVERLOADABLE void utest_type_printer(long long unsigned int i);
UBUT_FORCEINLINE UBUT_OVERLOADABLE void
utest_type_printer(long long unsigned int i) {
	UTEST_REZ_OUT("%llu", i);
}

//#ifdef __clang__
//#pragma clang diagnostic pop
//#endif

#endif

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)

UBUT_BEGIN_EXTERN_C

#define utest_type_printer(val)                                                \
  UTEST_REZ_OUT(_Generic((val), signed char                                     \
						: "%d", unsigned char                                  \
						: "%u", short                                          \
						: "%d", unsigned short                                 \
						: "%u", int                                            \
						: "%d", long                                           \
						: "%ld", long long                                     \
						: "%lld", unsigned                                     \
						: "%u", unsigned long                                  \
						: "%lu", unsigned long long                            \
						: "%llu", float                                        \
						: "%f", double                                         \
						: "%f", long double                                    \
						: "%Lf", default                                       \
						: _Generic((val - val), ptrdiff_t                      \
								   : "%p", default                             \
								   : "undef")),                                \
			   (val))

UBUT_END_EXTERN_C

#else
/*
   we don't have the ability to print the values we got, so we create a macro
   to tell our users we can't do anything fancy
*/
#define utest_type_printer(...) UTEST_REZ_OUT("undef")
#endif

#if defined(__cplusplus) && (__cplusplus >= 201103L)
#define UTEST_AUTO(x) auto
#elif !defined(__cplusplus)

#if defined(__clang__)
/* clang-format off */
/* had to disable clang-format here because it malforms the pragmas */
#define UTEST_AUTO(x)                                                          \
  _Pragma("clang diagnostic push")                                             \
	  _Pragma("clang diagnostic ignored \"-Wgnu-auto-type\"") __auto_type      \
		  _Pragma("clang diagnostic pop")
/* clang-format on */
#else
#define UTEST_AUTO(x) __auto_type
#endif

#else
#define UTEST_AUTO(x) typeof(x + 0)
#endif

#endif // UTEST_HAVING_TYPE_PRINTERS

#if defined(__clang__)
#define UTEST_STRNCMP(x, y, size)                                              \
  _Pragma("clang diagnostic push")                                             \
	  _Pragma("clang diagnostic ignored \"-Wdisabled-macro-expansion\"")       \
		  strncmp(x, y, size) _Pragma("clang diagnostic pop")
#else
#define UTEST_STRNCMP(x, y, size) ubut_strncmp(x, y, size)
#endif

#if defined(__clang__)
#define UTEST_EXPECT(x, y, cond)                                               \
  {                                                                            \
	_Pragma("clang diagnostic push")                                           \
		_Pragma("clang diagnostic ignored \"-Wlanguage-extension-token\"")     \
			_Pragma("clang diagnostic ignored \"-Wc++98-compat-pedantic\"")    \
				_Pragma("clang diagnostic ignored \"-Wfloat-equal\"")          \
					UTEST_AUTO(x) xEval = (x);                                 \
	UTEST_AUTO(y) yEval = (y);                                                 \
	if (!((xEval)cond(yEval))) {                                               \
	  _Pragma("clang diagnostic pop")                                          \
		  UTEST_REZ_OUT("%s:%u: Failure\n", __FILE__, __LINE__);                \
	  UTEST_REZ_OUT("  Expected : ");                                           \
	  utest_type_printer(xEval);                                               \
	  UTEST_REZ_OUT("\n");                                                      \
	  UTEST_REZ_OUT("    Actual : ");                                           \
	  utest_type_printer(yEval);                                               \
	  UTEST_REZ_OUT("\n");                                                      \
	  *utest_result = 1;                                                       \
	}                                                                          \
  } 

#else // ! __clang__

#define UTEST_EXPECT(x, y, cond)                                               \
  {                                                                            \
	if (!((x)cond(y))) {                                                       \
	  UTEST_REZ_OUT("%s:%u: Failure\n", __FILE__, __LINE__);                    \
	  *utest_result = 1;                                                       \
	}                                                                          \
  }

#endif // ! __clang__

#define EXPECT_TRUE(x)                                                         \
  if (!(x)) {                                                                  \
	UTEST_REZ_OUT("%s:%u: Failure\n", __FILE__, __LINE__);                      \
	UTEST_REZ_OUT("  Expected : true\n");                                       \
	UTEST_REZ_OUT("    Actual : %s\n", (x) ? "true" : "false");                 \
	*utest_result = 1;                                                         \
  }

#define EXPECT_FALSE(x)                                                        \
  if (x) {                                                                     \
	UTEST_REZ_OUT("%s:%u: Failure\n", __FILE__, __LINE__);                      \
	UTEST_REZ_OUT("  Expected : false\n");                                      \
	UTEST_REZ_OUT("    Actual : %s\n", (x) ? "true" : "false");                 \
	*utest_result = 1;                                                         \
  }

#define EXPECT_EQ(x, y) UTEST_EXPECT(x, y, ==)
#define EXPECT_NE(x, y) UTEST_EXPECT(x, y, !=)
#define EXPECT_LT(x, y) UTEST_EXPECT(x, y, <)
#define EXPECT_LE(x, y) UTEST_EXPECT(x, y, <=)
#define EXPECT_GT(x, y) UTEST_EXPECT(x, y, >)
#define EXPECT_GE(x, y) UTEST_EXPECT(x, y, >=)

#define EXPECT_STREQ(x, y)                                                     \
  if (0 != strcmp(x, y)) {                                                     \
	UTEST_REZ_OUT("%s:%u: Failure\n", __FILE__, __LINE__);                      \
	UTEST_REZ_OUT("  Expected : \"%s\"\n", x);                                  \
	UTEST_REZ_OUT("    Actual : \"%s\"\n", y);                                  \
	*utest_result = 1;                                                         \
  }

#define EXPECT_STRNE(x, y)                                                     \
  if (0 == strcmp(x, y)) {                                                     \
	UTEST_REZ_OUT("%s:%u: Failure\n", __FILE__, __LINE__);                      \
	UTEST_REZ_OUT("  Expected : \"%s\"\n", x);                                  \
	UTEST_REZ_OUT("    Actual : \"%s\"\n", y);                                  \
	*utest_result = 1;                                                         \
  }

#define EXPECT_STRNEQ(x, y)                                                    \
  if (0 != UTEST_STRNCMP(x, y, strlen(x))) {                                   \
	UTEST_REZ_OUT("%s:%u: Failure\n", __FILE__, __LINE__);                      \
	UTEST_REZ_OUT("  Expected : \"%s\"\n", x);                                  \
	UTEST_REZ_OUT("    Actual : \"%s\"\n", y);                                  \
	*utest_result = 1;                                                         \
  }

#define EXPECT_STRNNE(x, y)                                                    \
  if (0 == UTEST_STRNCMP(x, y, strlen(x))) {                                   \
	UTEST_REZ_OUT("%s:%u: Failure\n", __FILE__, __LINE__);                      \
	UTEST_REZ_OUT("  Expected : \"%s\"\n", x);                                  \
	UTEST_REZ_OUT("    Actual : \"%s\"\n", y);                                  \
	*utest_result = 1;                                                         \
  }

#if defined(__clang__)
#define UTEST_ASSERT(x, y, cond)                                               \
  {                                                                            \
	_Pragma("clang diagnostic push")                                           \
		_Pragma("clang diagnostic ignored \"-Wlanguage-extension-token\"")     \
			_Pragma("clang diagnostic ignored \"-Wc++98-compat-pedantic\"")    \
				_Pragma("clang diagnostic ignored \"-Wfloat-equal\"")          \
					UTEST_AUTO(x) xEval = (x);                                 \
	UTEST_AUTO(y) yEval = (y);                                                 \
	if (!((xEval)cond(yEval))) {                                               \
	  _Pragma("clang diagnostic pop")                                          \
		  UTEST_REZ_OUT("%s:%u: Failure\n", __FILE__, __LINE__);                \
	  UTEST_REZ_OUT("  Expected : ");                                           \
	  utest_type_printer(xEval);                                               \
	  UTEST_REZ_OUT("\n");                                                      \
	  UTEST_REZ_OUT("    Actual : ");                                           \
	  utest_type_printer(yEval);                                               \
	  UTEST_REZ_OUT("\n");                                                      \
	  *utest_result = 1;                                                       \
	  return;                                                                  \
	}                                                                          \
  }
#else // ! __clang__ 
#define UTEST_ASSERT(x, y, cond)                                               \
  {                                                                            \
	if (!((x)cond(y))) {                                                       \
	  UTEST_REZ_OUT("%s:%u: Failure\n", __FILE__, __LINE__);                    \
	  *utest_result = 1;                                                       \
	  return;                                                                  \
	}                                                                          \
  }

#endif // ! __clang__

#define ASSERT_TRUE(x)                                                         \
  if (!(x)) {                                                                  \
	UTEST_REZ_OUT("%s:%u: Failure\n", __FILE__, __LINE__);                      \
	UTEST_REZ_OUT("  Expected : true\n");                                       \
	UTEST_REZ_OUT("    Actual : %s\n", (x) ? "true" : "false");                 \
	*utest_result = 1;                                                         \
	return;                                                                    \
  }

#define ASSERT_FALSE(x)                                                        \
  if (x) {                                                                     \
	UTEST_REZ_OUT("%s:%u: Failure\n", __FILE__, __LINE__);                      \
	UTEST_REZ_OUT("  Expected : false\n");                                      \
	UTEST_REZ_OUT("    Actual : %s\n", (x) ? "true" : "false");                 \
	*utest_result = 1;                                                         \
	return;                                                                    \
  }

#define ASSERT_EQ(x, y) UTEST_ASSERT(x, y, ==)
#define ASSERT_NE(x, y) UTEST_ASSERT(x, y, !=)
#define ASSERT_LT(x, y) UTEST_ASSERT(x, y, <)
#define ASSERT_LE(x, y) UTEST_ASSERT(x, y, <=)
#define ASSERT_GT(x, y) UTEST_ASSERT(x, y, >)
#define ASSERT_GE(x, y) UTEST_ASSERT(x, y, >=)

#define ASSERT_STREQ(x, y)                                                     \
  if (0 != strcmp(x, y)) {                                                     \
	UTEST_REZ_OUT("%s:%u: Failure\n", __FILE__, __LINE__);                      \
	UTEST_REZ_OUT("  Expected : \"%s\"\n", x);                                  \
	UTEST_REZ_OUT("    Actual : \"%s\"\n", y);                                  \
	*utest_result = 1;                                                         \
	return;                                                                    \
  }

#define ASSERT_STRNE(x, y)                                                     \
  if (0 == strcmp(x, y)) {                                                     \
	UTEST_REZ_OUT("%s:%u: Failure\n", __FILE__, __LINE__);                      \
	UTEST_REZ_OUT("  Expected : \"%s\"\n", x);                                  \
	UTEST_REZ_OUT("    Actual : \"%s\"\n", y);                                  \
	*utest_result = 1;                                                         \
	return;                                                                    \
  }

#define ASSERT_STRNEQ(x, y)                                                    \
  if (0 != UTEST_STRNCMP(x, y, strlen(x))) {                                   \
	UTEST_REZ_OUT("%s:%u: Failure\n", __FILE__, __LINE__);                      \
	UTEST_REZ_OUT("  Expected : \"%s\"\n", x);                                  \
	UTEST_REZ_OUT("    Actual : \"%s\"\n", y);                                  \
	*utest_result = 1;                                                         \
	return;                                                                    \
  }

#define ASSERT_STRNNE(x, y)                                                    \
  if (0 == UTEST_STRNCMP(x, y, strlen(x))) {                                   \
	UTEST_REZ_OUT("%s:%u: Failure\n", __FILE__, __LINE__);                      \
	UTEST_REZ_OUT("  Expected : \"%s\"\n", x);                                  \
	UTEST_REZ_OUT("    Actual : \"%s\"\n", y);                                  \
	*utest_result = 1;                                                         \
	return;                                                                    \
  }

#define UTEST(SET, NAME)                                                       \
  /*struct utest_state_s utest_state;*/                               \
static void utest_run_##SET##_##NAME(int *utest_result);                     \
  static void utest_##SET##_##NAME(int *utest_result, size_t utest_index) {    \
	(void)utest_index;                                                         \
	utest_run_##SET##_##NAME(utest_result);                                    \
  }                                                                            \
  UBUT_INITIALIZER(utest_register_##SET##_##NAME) {                           \
	const size_t index = utest_state.tests_length++;                           \
	const char *name_part = #SET "." #NAME;                                    \
	const size_t name_size = strlen(name_part) + 1;                            \
	char *name = UBUT_PTR_CAST(char *, malloc(name_size));                    \
	utest_state.tests =                                                        \
		UBUT_PTR_CAST(struct utest_test_state_s *,                            \
					   realloc(UBUT_PTR_CAST(void *, utest_state.tests),      \
							   sizeof(struct utest_test_state_s) *             \
								   utest_state.tests_length));                 \
	utest_state.tests[index].func = &utest_##SET##_##NAME;                     \
	utest_state.tests[index].name = name;                                      \
	utest_state.tests[index].test_index = index;                              \
	UBUT_SNPRINTF(name, name_size, "%s", name_part);                          \
  }                                                                            \
  void utest_run_##SET##_##NAME(int *utest_result)

UBUT_BEGIN_EXTERN_C

 int utest_should_filter_test(const char* filter, const char* testcase);

#ifdef UTEST_IMPLEMENTATION

 int utest_should_filter_test(const char* filter,
	const char* testcase) {
	if (filter) {
		const char* filter_cur = filter;
		const char* testcase_cur = testcase;
		const char* filter_wildcard = UBUT_NULL;

		while (('\0' != *filter_cur) && ('\0' != *testcase_cur)) {
			if ('*' == *filter_cur) {
				/* store the position of the wildcard */
				filter_wildcard = filter_cur;

				/* skip the wildcard character */
				filter_cur++;

				while (('\0' != *filter_cur) && ('\0' != *testcase_cur)) {
					if ('*' == *filter_cur) {
						/*
						   we found another wildcard (filter is something like *foo*) so we
						   exit the current loop, and return to the parent loop to handle
						   the wildcard case
						*/
						break;
					}
					else if (*filter_cur != *testcase_cur) {
						/* otherwise our filter didn't match, so reset it */
						filter_cur = filter_wildcard;
					}

					/* move testcase along */
					testcase_cur++;

					/* move filter along */
					filter_cur++;
				}

				if (('\0' == *filter_cur) && ('\0' == *testcase_cur)) {
					return 0;
				}

				/* if the testcase has been exhausted, we don't have a match! */
				if ('\0' == *testcase_cur) {
					return 1;
				}
			}
			else {
				if (*testcase_cur != *filter_cur) {
					/* test case doesn't match filter */
					return 1;
				}
				else {
					/* move our filter and testcase forward */
					testcase_cur++;
					filter_cur++;
				}
			}
		}

		if (('\0' != *filter_cur) ||
			(('\0' != *testcase_cur) &&
				((filter == filter_cur) || ('*' != filter_cur[-1])))) {
			/* we have a mismatch! */
			return 1;
		}
	}

	return 0;
}
#endif // UTEST_IMPLEMENTATION

/* Informational switches */
#define HELP_STR "--help"
#define LIST_STR "--list"
/* Benchmark config switches */
#define FILTER_STR "--filter="
#define OUTPUT_STR "--output="
#define SLEN(S) (sizeof(S) - 1)

#define PFX_DIVIDER "[----------]"
#define     PFX_RUN "[ RUN      ]"
#define  PFX_FAILED UBUT_VT_YELLOW "[  FAILED  ]" UBUT_VT_RESET
#define  PFX_PASSED "[  PASSED  ]"
#define      PFX_OK "[      OK  ]"

 int utest_main(int argc, const char* const argv[]);
#ifdef UTEST_IMPLEMENTATION
 int utest_main(int argc, const char* const argv[]) {
	ubut_uint64_t failed = 0;
	size_t* failed_testcases = UBUT_NULL;
	size_t failed_testcases_length = 0;
	const char* filter = UBUT_NULL;
	ubut_uint64_t ran_tests = 0;

	/* loop through all arguments looking for our options */
	for (size_t index = 1; index < UBUT_CAST(size_t, argc); index++) {

		if (0 == ubut_strncmp(argv[index], HELP_STR, SLEN(HELP_STR))) {
			UBUT_INFO("utest - the testing solution");
			UBUT_INFO("Command line Options:");
			UBUT_INFO("  --help            Show this message and exit.");
			UBUT_INFO("  --filter=<filter> Filter the test cases to run (EG. MyTest*.a ");
			UBUT_INFO("would run MyTestCase.a but not MyTestCase.b).");
			UBUT_INFO("  %s      List testnames, one per line. Output names ", LIST_STR);
			UBUT_INFO("can be passed to --filter.");
			UBUT_INFO("  --output=<output> Output an xunit XML file to the file ");
			UBUT_INFO("specified in <output>.");
			goto cleanup;
		}
		else if (0 ==
			ubut_strncmp(argv[index], FILTER_STR, SLEN(FILTER_STR))) {
			/* user wants to filter what test cases run! */
			filter = argv[index] + SLEN(FILTER_STR);
		}
		else if (0 ==
			ubut_strncmp(argv[index], OUTPUT_STR, SLEN(OUTPUT_STR))) {
			utest_state.output = ubut_fopen(argv[index] + SLEN(OUTPUT_STR), "w+");
		}
		else if (0 == ubut_strncmp(argv[index], LIST_STR, SLEN(LIST_STR))) {
			// user want to list the tests and leave
			UBUT_INFO(" ");
			UBUT_INFO("List of tests");
			UBUT_INFO(" ");
			for (index = 0; index < utest_state.tests_length; index++) {
				UBUT_INFO("%-4d: %s", (int)index, utest_state.tests[index].name);
			}
			UBUT_INFO(" ");
			UBUT_INFO("To run exact test or group, filtered by name, use the --filter option");
			UBUT_INFO(" ");
			/* when printing the test list, don't actually run the tests */
			return 0;
		}
	}

	for (size_t index = 0; index < utest_state.tests_length; index++) {
		if (utest_should_filter_test(filter, utest_state.tests[index].name)) {
			continue;
		}

		ran_tests++;
	}

	UTEST_REZ_OUT(PFX_DIVIDER "Running %" UBUT_PRIu64 " test cases.",
		UBUT_CAST(ubut_uint64_t, ran_tests));

	UTEST_XML_OUT("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	UTEST_XML_OUT("<testsuites tests=\"%" UBUT_PRIu64 "\" name=\"All\">\n", UBUT_CAST(ubut_uint64_t, ran_tests));
	UTEST_XML_OUT("<testsuite name=\"Tests\" tests=\"%" UBUT_PRIu64 "\">\n", UBUT_CAST(ubut_uint64_t, ran_tests));

	for (size_t index_ = 0; index_ < utest_state.tests_length; index_++) {
		int result = 0;
		ubut_int64_t nanosecs_ = 0;

		if (utest_should_filter_test(filter, utest_state.tests[index_].name)) {
			continue;
		}

		UTEST_REZ_OUT(PFX_RUN "%s", utest_state.tests[index_].name);

		UTEST_XML_OUT("<testcase name=\"%s\">", utest_state.tests[index_].name);

		nanosecs_ = ubut_ns();
		utest_state.tests[index_].func(&result, utest_state.tests[index_].test_index);
		nanosecs_ = ubut_ns() - nanosecs_;

		UTEST_XML_OUT("</testcase>\n");

		if (0 != result) {
			const size_t failed_testcase_index = failed_testcases_length++;

#pragma warning( push )
#pragma warning( disable : 6308 )

			failed_testcases = UBUT_PTR_CAST(
				size_t*, realloc(UBUT_PTR_CAST(void*, failed_testcases),
					sizeof(size_t) * failed_testcases_length));

#pragma warning( pop )

			failed_testcases[failed_testcase_index] = index_;
			failed++;
			UTEST_REZ_OUT(PFX_FAILED "%s (%" UBUT_PRId64 " nanosecs)", utest_state.tests[index_].name, nanosecs_);
		}
		else {
			UTEST_REZ_OUT(PFX_OK "%s (%" UBUT_PRId64 " nanosecs)", utest_state.tests[index_].name, nanosecs_);
		}
	} // for()

	UTEST_REZ_OUT(PFX_DIVIDER "%" UBUT_PRIu64 " test cases ran.", ran_tests);
	UTEST_REZ_OUT(PFX_PASSED "%" UBUT_PRIu64 " tests.", ran_tests - failed);

	if (0 != failed) {
		UBUT_WARN(PFX_FAILED "%" UBUT_PRIu64 " tests, listed below:", failed);
		for (size_t index = 0; index < failed_testcases_length; index++) {
			UBUT_WARN(PFX_FAILED "%s", utest_state.tests[failed_testcases[index]].name);
		}
	}

	UTEST_XML_OUT("</testsuite>\n</testsuites>\n");

cleanup:
	for (size_t index = 0; index < utest_state.tests_length; index++) {
		free(UBUT_PTR_CAST(void*, utest_state.tests[index].name));
	}

	free(UBUT_PTR_CAST(void*, failed_testcases));
	free(UBUT_PTR_CAST(void*, utest_state.tests));

	if (utest_state.output) {
		fclose(utest_state.output);
	}

	UBUT_CONSOLE_COLOR_RESET();

	return UBUT_CAST(int, failed);
} // utest_main
#endif // UTEST_IMPLEMENTATION
#undef HELP_STR
#undef LIST_STR
#undef FILTER_STR
#undef OUTPUT_STR
#undef CONFIDENCE_STR
#undef SLEN

#undef PFX_DIVIDER
#undef     PFX_RUN
#undef  PFX_FAILED
#undef  PFX_PASSED
#undef      PFX_OK

/*
   we need, in exactly one source file, define the global struct that will hold
   the data we need to run utest. This macro allows the user to declare the
   data without having to use the UTEST_MAIN macro, thus allowing them to write
   their own main() function.

   DBJ warning:

   had a persistent "function not declared" from cl.exe when using UTEST_STATE()
   removed `()`
   UTEST_STATE ... no warnings

*/
#ifdef UTEST_IMPLEMENTATION

#define UTEST_STATE struct utest_state_s utest_state = { NULL , 0, 0}

/*
   define a main() function to call into utest.h and start executing tests! A
   user can optionally not use this macro, and instead define their own main()
   function and manually call utest_main. The user must, in exactly one source
   file, use the UTEST_STATE macro to declare a global struct variable that
   utest requires.

#define UTEST_MAIN()                        \
  UTEST_STATE;   // <-- note there is no ()! \
  int main(int argc, const char *const argv[]) {\
	return utest_main(argc, argv);          \
  }
*/
#endif // UTEST_IMPLEMENTATION

UBUT_END_EXTERN_C

#endif /* SHEREDOM_UTEST_H_INCLUDED */
