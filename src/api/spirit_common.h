/*
 * spirit_common.h
 *
 *  Created on: 04.08.2011
 *      Author: delbertooo
 */

#ifndef SPIRIT_COMMON_H_
#define SPIRIT_COMMON_H_

//includes

#ifdef __cplusplus
extern "C" {
#endif

#if (defined(_WIN32) || defined(WIN32))
	#ifdef libspirit_EXPORTS
		#define LIBSPIRIT_API __declspec(dllexport)
	#else
		#define LIBSPIRIT_API __declspec(dllimport)
	#endif
#else /* not MSVC */
	#define LIBSPIRIT_API
#endif






#ifdef __cplusplus
}
#endif


#endif /* SPIRIT_COMMON_H_ */
