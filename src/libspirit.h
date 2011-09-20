/*
 * libspirit.h
 *
 *  Created on: 11.08.2011
 *      Author: delbertooo
 */

#ifndef LIBSPIRIT_H_
#define LIBSPIRIT_H_

#include <stdarg.h>
#include <curl/curl.h>
#include "libspirit_data.h"
#include "utility.h"

SPIRITcode Spirit_initLibcurlSettings(struct LibcurlSettings *curl);
SPIRITcode Spirit_initCurlConnectionForUrl(struct SpiritHandle *spirit, CURL **curl_handle, const char *url, struct MemoryStruct *chunk);


#endif /* LIBSPIRIT_H_ */
