/*
 * spirit_error.h
 *
 *  Created on: 10.08.2011
 *      Author: delbertooo
 */

#ifndef SPIRIT_ERROR_H_
#define SPIRIT_ERROR_H_

#include <libspirit/spirit.h>

#ifdef __cplusplus
extern "C" {
#endif

LIBSPIRIT_API const char *spirit_errstring(SPIRITcode error);

#ifdef __cplusplus
}
#endif

#endif /* SPIRIT_ERROR_H_ */
