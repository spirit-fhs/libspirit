/*
 * libspirit.h
 *
 *  Created on: 04.08.2011
 *      Author: delbertooo
 */

#include <libspirit/libspirit_common.h>

#ifndef LIBSPIRIT_H_
#define LIBSPIRIT_H_

#ifdef __cplusplus
extern "C" {
#endif



LIBSPIRIT_API typedef enum {
  SPIRITE_OK = 0,
  SPIRITE_OUT_OF_MEMORY,         /* 1  - memory allocation failed */
  SPIRITE_OBSOLETE2,             /* 2  - NOT USED */

  SPIRIT_LAST /* never use! */
} SPIRITcode;

LIBSPIRIT_API typedef void SPIRIT;

LIBSPIRIT_API int fndlltest(void);

LIBSPIRIT_API int curltest(char* url);

LIBSPIRIT_API SPIRITcode spirit_news_print_all(SPIRIT *handle);
LIBSPIRIT_API SPIRIT *spirit_init(const char *base_url);





#ifdef __cplusplus
}
#endif


#endif /* LIBSPIRIT_H_ */
