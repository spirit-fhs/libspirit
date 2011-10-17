/*
 * libspirit_schedule.c
 *
 *  Created on: 17.10.2011
 *      Author: delbertooo
 */


#include <stdio.h>

#include <curl_config.h>
#include <curl/curl.h>
#include <yajl/yajl_tree.h>
#include <libspirit/spirit.h>

#include "libspirit.h"

/* prints all news */
LIBSPIRIT_API SPIRITcode spirit_schedule_print(SPIRIT *handle, char *classname, char week)
{
	struct SpiritHandle *data = (struct SpiritHandle *)handle;
	CURL *curl;
	struct MemoryStruct chunk;
	SPIRITcode res = SPIRITE_OK;
	const char* request_pattern = "schedule?classname=%s&week=%c";
	char *request_url;

	request_url = calloc(strlen(request_pattern) + strlen(classname) + 1 - 4, sizeof(char));
	request_url = sprintf(request_pattern, classname, week);

	chunk.memory = malloc(1); /* will be grown as needed by the realloc above */
	chunk.size = 0; /* no data at this point */

	if (chunk.memory == NULL)
		return SPIRITE_OUT_OF_MEMORY;

	res = Spirit_initCurlConnectionForUrl(handle, &curl, request_url, &chunk);

	curl_easy_perform(curl);
	/* always cleanup -- did we cleanup the slist too? */
	curl_easy_cleanup(curl);

	if (res != SPIRITE_OK) {
		if (chunk.memory)
			free(chunk.memory);
		return res;
	}

	//res = Spirit_printNewsFromJsonStringNew(chunk.memory);
	printf(chunk.memory);

	if (chunk.memory)
		free(chunk.memory);

	return res;
}
