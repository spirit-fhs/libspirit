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

/* prints all events from json string for the new REST service */
static SPIRITcode Spirit_printScheduleFromJsonString(const char* json)
{
	yajl_val node;
	char errbuf[1024];
	SPIRITcode res = SPIRITE_OK;

	node = yajl_tree_parse((const char *) json, errbuf, sizeof(errbuf));

	/* parse error handling */
	if (node == NULL)
		return SPIRITE_JSON_PARSE_ERROR;

	/* print news */
	{
		if (YAJL_IS_ARRAY(node)) {
			yajl_val *allNews = node->u.array.values;
			unsigned int i;
			for (i = 0; i < node->u.array.len; ++i) {
				const char * pathTitle[] = { "subject", (const char *) 0 };
				const char * pathContent[] = { "news", (const char *) 0 };

				yajl_val title = yajl_tree_get(*(allNews), pathTitle, yajl_t_string);
				yajl_val content = yajl_tree_get(*(allNews), pathContent, yajl_t_string);

				if (YAJL_IS_STRING(title) && YAJL_IS_STRING(content)) {
					printf("--[ %s ]", YAJL_GET_STRING(title));
					fprintNChars(stdout, '-', 80 - strlen(YAJL_GET_STRING(title)) - 6);
					printf("\n%s\n", YAJL_GET_STRING(content));
					fprintNChars(stdout, '-', 80);
					printf("\n\n");
				}

				++allNews;
			}



		}
		else
			return SPIRITE_JSON_NODE_NOT_FOUND;
	}

	yajl_tree_free(node);

	return res;
}

/* prints all events for a course  */
LIBSPIRIT_API SPIRITcode spirit_schedule_print(SPIRIT *handle, char *classname, char week)
{
	struct SpiritHandle *data = (struct SpiritHandle *)handle;
	CURL *curl;
	struct MemoryStruct chunk;
	SPIRITcode res = SPIRITE_OK;
	const char* request_pattern = "schedule?classname=%s&week=%c";
	char *request_url;

	request_url = calloc(strlen(request_pattern) - 4 + strlen(classname) + 1, sizeof(char));
	sprintf(request_url, request_pattern, classname, week);

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

	res = Spirit_printScheduleFromJsonString(chunk.memory);
	//printf(chunk.memory);

	if (chunk.memory)
		free(chunk.memory);

	return res;
}
