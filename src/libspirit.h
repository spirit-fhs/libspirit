/*
 * libspirit.h
 *
 *  Created on: 11.08.2011
 *      Author: delbertooo
 */

#ifndef LIBSPIRIT_H_
#define LIBSPIRIT_H_

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <curl/curl.h>

#include "libspirit_data.h"
#include "utility.h"

SPIRITcode Spirit_initLibcurlSettings(struct LibcurlSettings *curl);
SPIRITcode Spirit_initCurlConnectionForUrl(struct SpiritHandle *spirit, CURL **curl_handle, const char *url, struct MemoryStruct *chunk);
SPIRITcode Spirit_setopt(struct SpiritSession *data, SPIRIToption option, va_list param);

#ifdef __cplusplus
}
#endif


#endif /* LIBSPIRIT_H_ */
