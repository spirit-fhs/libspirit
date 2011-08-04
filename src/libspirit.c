/*
 * libspirit.c
 *
 *  Created on: 01.08.2011
 *      Author: delbertooo
 */

#include <libspiritconfig.h>
#include <yajl/yajl_gen.h>
#include <yajl/yajl_parse.h>
#include <curl_config.h>
#include <curl/curl.h>

#include <libspirit/libspirit.h>


LIBSPIRIT_API int fndlltest(void)
{
	yajl_gen g;
	yajl_status st;
	yajl_handle h;
	st = yajl_parse(h, "[]", 3);
	return 42;
}

LIBSPIRIT_API int curltest(void)
{
	CURL *curl;
	CURLcode res;

	curl = curl_easy_init();
	if(curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
		res = curl_easy_perform(curl);

		/* always cleanup */
		curl_easy_cleanup(curl);
	}
	return 0;
}
