#ifndef _libspirit_libspirit_h_
#define _libspirit_libspirit_h_

#ifdef WIN32
	#ifdef libspirit_EXPORTS
		#define LIBSPIRIT_API __declspec(dllexport)
	#else
		#define LIBSPIRIT_API __declspec(dllimport)
	#endif
#else /* not MSVC */
	#define LIBSPIRIT_API
#endif


/* make sure the lib is callable from C++ */
#ifdef _cplusplus
#define BEGIN_DECLS extern "C" {
#define END_DECLS }
#else
#define BEGIN_DECLS
#define END_DECLS
#endif

/* PARAMS is a macro used to wrap function prototypes, so that compilers that
don't understand ANSI C prototypes still work, and ANSI C compilers can
issue warnings about type mismatches.
#ifdef PARAMS
#undef PARAMS
#endif
#if defined (__STDC__) || defined (_AIX) \
|| (defined (__mips) && defined (_SYSTYPE_SVR4)) \
|| defined(WIN32) || defined(__cplusplus)
#define PARAMS(protos) protos
#else
#define PARAMS(protos) ()
#endif*/

#endif /* _libspirit_libspirit_h_ */
