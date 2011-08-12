/*
 * libspirit_option.h
 *
 *  Created on: 12.08.2011
 *      Author: delbertooo
 */

#ifndef LIBSPIRIT_OPTION_H_
#define LIBSPIRIT_OPTION_H_

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <libspirit/spirit.h>

#include "libspirit_data.h"
#include "utility.h"

SPIRITcode Spirit_setopt(struct SpiritHandle *data, SPIRIToption option, va_list param);


#ifdef __cplusplus
}
#endif

#endif /* LIBSPIRIT_OPTION_H_ */
