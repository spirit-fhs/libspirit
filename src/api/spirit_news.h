/*
 * spirit_news.h
 *
 *  Created on: 11.08.2011
 *      Author: delbertooo
 */

#ifndef SPIRIT_NEWS_H_
#define SPIRIT_NEWS_H_

#include <libspirit/spirit.h>

#ifdef __cplusplus
extern "C" {
#endif

LIBSPIRIT_API SPIRITcode spirit_news_print_all(SPIRIT *handle);
LIBSPIRIT_API SPIRITcode spirit_news_by_date(SPIRIT *handle, char **mem, size_t *size);

#ifdef __cplusplus
}
#endif

#endif /* SPIRIT_NEWS_H_ */
