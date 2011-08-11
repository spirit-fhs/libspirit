/*
 * libspirit_news.c
 *
 *  Created on: 11.08.2011
 *      Author: delbertooo
 */

#include <stdio.h>

#include <curl/curl.h>
#include <yajl/yajl_tree.h>

#include <libspirit/spirit.h>

#include "libspirit.h"
#include "utility.h"



static SPIRITcode Spirit_printNewsFromJsonString(const char* json) {
	yajl_val node;
	char errbuf[1024];
	SPIRITcode res = SPIRITE_OK;

	node = yajl_tree_parse((const char *) json, errbuf, sizeof(errbuf));

	/* parse error handling */
	if (node == NULL)
		return SPIRITE_JSON_PARSE_ERROR;

	/* print news */
	{
		const char * path[] = { "news", (const char *) 0 };
		yajl_val newsNode = yajl_tree_get(node, path, yajl_t_array);
		if (YAJL_IS_ARRAY(newsNode)) {
			yajl_val *allNews = newsNode->u.array.values;
			unsigned int i;
			for (i = 0; i < newsNode->u.array.len; ++i) {
				const char * pathTitle[] = { "title", (const char *) 0 };
				const char * pathContent[] = { "content", (const char *) 0 };

				yajl_val title = yajl_tree_get(*(allNews), pathTitle, yajl_t_string);
				yajl_val content = yajl_tree_get(*(allNews), pathContent, yajl_t_string);

				if (YAJL_IS_STRING(title) && YAJL_IS_STRING(content)) {
					printf("--[ %s ]", YAJL_GET_STRING(title));
					fprintNChars(stdout, '-', 80 - strlen(YAJL_GET_STRING(title)) - 6);
					printf("\n%s\n", YAJL_GET_STRING(content));
					fprintNChars(stdout, '-', 80);
					printf("\n\n");
				}
				/*else
					printf("no such node: %s\n", path[0]);*/

				++allNews;
			}



		}
		else
			/*printf("no such node: %s\n", path[0]);*/
			return SPIRITE_JSON_NODE_NOT_FOUND;
	}

	yajl_tree_free(node);

	return res;
}

LIBSPIRIT_API SPIRITcode spirit_news_print_all(SPIRIT *handle) {
	struct SpiritHandle *data = (struct SpiritHandle *)handle;
	CURL *curl;
	struct MemoryStruct chunk;
	SPIRITcode res = SPIRITE_OK;

	chunk.memory = malloc(1); /* will be grown as needed by the realloc above */
	chunk.size = 0; /* no data at this point */

	if (chunk.memory == NULL)
		return SPIRITE_OUT_OF_MEMORY;

	res = Spirit_initCurlConnectionForUrl(handle, &curl, "news", &chunk);

	curl_easy_perform(curl);
	/* always cleanup -- did we cleanup the slist too? */
	curl_easy_cleanup(curl);

	if (res != SPIRITE_OK) {
		if (chunk.memory)
			free(chunk.memory);
		return res;
	}

	res = Spirit_printNewsFromJsonString(chunk.memory);

	if (chunk.memory)
		free(chunk.memory);

	return res;
}
