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

#define SKIP_PEER_VERIFICATION DEFINED
#define SKIP_HOSTNAME_VERIFICATION DEFINED


LIBSPIRIT_API typedef void SPIRIT;

LIBSPIRIT_API int fndlltest(void);

LIBSPIRIT_API int curltest(char* url);






#ifdef __cplusplus
}
#endif


#endif /* LIBSPIRIT_H_ */
