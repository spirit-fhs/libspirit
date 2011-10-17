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
				const char * pathName[] = { "titleShort", (const char *) 0 };
				const char * pathType[] = { "eventType", (const char *) 0 };
				const char * pathTime[] = { "appointment", "time", (const char *) 0 };
				const char * pathWeek[] = { "appointment", "week", (const char *) 0 };
				const char * pathBuilding[] = { "appointment", "location", "place", "building", (const char *) 0 };
				const char * pathRoom[] = { "appointment", "location", "place", "room", (const char *) 0 };
				/*const char * pathMember[] = { "member", "name", (const char *) 0 };*/

				yajl_val name = yajl_tree_get(*(allNews), pathName, yajl_t_string);
				yajl_val type = yajl_tree_get(*(allNews), pathType, yajl_t_string);
				yajl_val time = yajl_tree_get(*(allNews), pathTime, yajl_t_string);
				yajl_val week = yajl_tree_get(*(allNews), pathWeek, yajl_t_string);
				yajl_val building = yajl_tree_get(*(allNews), pathBuilding, yajl_t_string);
				yajl_val room = yajl_tree_get(*(allNews), pathRoom, yajl_t_string);
				/*yajl_val member = yajl_tree_get(*(allNews), pathMember, yajl_t_string);*/

				if (YAJL_IS_STRING(name) && YAJL_IS_STRING(type) && YAJL_IS_STRING(time) && YAJL_IS_STRING(week) && YAJL_IS_STRING(room) && YAJL_IS_STRING(building)) {
					printf("%s [%s] im Raum [%s-%s] in der Zeitspanne [%s][%s] bei [%s].\n\n",
							YAJL_GET_STRING(type),
							YAJL_GET_STRING(name),
							YAJL_GET_STRING(building),
							YAJL_GET_STRING(room),
							YAJL_GET_STRING(time),
							YAJL_GET_STRING(week),
							"DUMMY"/*YAJL_GET_STRING(member)*/);
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
