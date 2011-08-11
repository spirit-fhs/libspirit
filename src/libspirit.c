/*
 * libspirit.c
 *
 *  Created on: 01.08.2011
 *      Author: delbertooo
 */

#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>

#include <libspiritconfig.h>
#include <yajl/yajl_tree.h>
#include <curl_config.h>
#include <curl/curl.h>

#include <libspirit/spirit.h>

#include "libspirit_data.h"


#define SKIP_PEER_VERIFICATION DEFINED
#define SKIP_HOSTNAME_VERIFICATION DEFINED

struct MemoryStruct {
	char *memory;
	size_t size;
};



static size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb,
		void *data) {
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *) data;

	mem->memory = realloc(mem->memory, mem->size + realsize + 1);
	if (mem->memory == NULL) {
		/* out of memory! */
		printf("not enough memory (realloc returned NULL)\n");
		exit(EXIT_FAILURE);
	}

	memcpy(&(mem->memory[mem->size]), ptr, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}


static SPIRITcode Spirit_initLibcurlSettings(struct LibcurlSettings *curl) {
	SPIRITcode res = SPIRITE_OK;

	curl->header_accept = "Accept: application/json";
	curl->ssl_cipher_type = "DHE-RSA-AES256-SHA";
	curl->user_agent = libspirit_USER_AGENT;

	return res;
}

static SPIRITcode Spirit_initCurlConnectionForUrl(struct SpiritHandle *spirit, CURL **curl_handle, const char *url, struct MemoryStruct *chunk) {
	struct curl_slist *slist = NULL;
	char *request_url;
	SPIRITcode res = SPIRITE_OK;
	CURL *curl;

	request_url = calloc(strlen(spirit->base_url) + strlen(url) + 1, sizeof(char));
	if (request_url == NULL)
		return SPIRITE_OUT_OF_MEMORY;

	strcpy(request_url, spirit->base_url);
	strcpy(&(request_url[strlen(request_url)]), url);



	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, request_url);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);

#ifdef SKIP_PEER_VERIFICATION
		/* connect to a site who isn't using a certificate that is signed by one of the certs in the CA bundle you have */
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif

#ifdef SKIP_HOSTNAME_VERIFICATION
		/* skip check if site you're connecting uses a different host name to what they have mentioned in their server certificate's commonName */
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif
		curl_easy_setopt(curl, CURLOPT_SSL_CIPHER_LIST, spirit->curl.ssl_cipher_type);
		curl_easy_setopt( curl, CURLOPT_USERAGENT, spirit->curl.user_agent);

		slist = curl_slist_append(slist, spirit->curl.header_accept);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);

		/* send all data to this function  */
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

		/* we pass our 'chunk' struct to the callback function */
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)chunk);
	}
	*curl_handle = curl;

	return res;
}

static void fprintNChars(FILE *file, char c, unsigned int n) {
	unsigned int i;
	for (i = 0; i < n; ++i)
		fprintf(file, "%c", c);
}

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
				//else
				//	printf("no such node: %s\n", path[0]);

				++allNews;
			}



		}
		else
			//printf("no such node: %s\n", path[0]);
			return SPIRITE_JSON_NODE_NOT_FOUND;
	}

	yajl_tree_free(node);

	return res;
}

LIBSPIRIT_API SPIRIT *spirit_init(const char *base_url) {
	struct SpiritHandle *handle;
	handle = calloc(1, sizeof(struct SpiritHandle));
	if (handle == NULL)
			return NULL;

	Spirit_initLibcurlSettings(&handle->curl);

	handle->base_url = base_url;





	return handle;
}

LIBSPIRIT_API void spirit_cleanup(SPIRIT *handle) {
	struct SpiritHandle *data = (struct SpiritHandle *)handle;

	if (data)
		free(data);
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

	//printf(chunk.memory);
	res = Spirit_printNewsFromJsonString(chunk.memory);

	if (chunk.memory)
		free(chunk.memory);

	return res;
}


