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

#include <libspirit/libspirit.h>

#include "spirit_data.h"


#define STR_VALUE(arg) #arg
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


LIBSPIRIT_API int fndlltest(void) {
//	yajl_gen g;
//	yajl_status st;
//	yajl_handle h;
//	yajl_config(h, yajl_allow_comments, 1);
//	st = yajl_parse(h, "{}", 2);
//	return 42;
	static unsigned char fileData[65536];

	size_t rd;
	yajl_val node;
	char errbuf[1024];
	FILE *fp;

	/* null plug buffers */
	fileData[0] = errbuf[0] = 0;

	/* read the entire config file */
	if((fp = fopen("sample.config", "r"))==NULL) {
	    printf("Cannot open file.\n");
	    return 1;
	  }

	//rd = fread((void *) fileData, 1, sizeof(fileData) - 1, stdin);
	rd = fread((void *) fileData, 1, sizeof(fileData) - 1, fp);
	puts(fileData);


	/* file read error handling */
	if (rd == 0 && !feof(fp)) {
		fprintf(stderr, "error encountered on file read\n");
		return 1;
	} else if (rd >= sizeof(fileData) - 1) {
		fprintf(stderr, "config file too big\n");
		return 1;
	}

	/* we have the whole config file in memory.  let's parse it ... */
	node = yajl_tree_parse((const char *) fileData, errbuf, sizeof(errbuf));

	/* parse error handling */
	if (node == NULL) {
		fprintf(stderr, "parse_error: ");
		if (strlen(errbuf))
			fprintf(stderr, " %s", errbuf);
		else
			fprintf(stderr, "unknown error");
		fprintf(stderr, "\n");
		return 1;
	}

	/* ... and extract a nested value from the config file */
	{
		const char * path[] = { "Logging", "timeFormat", (const char *) 0 };
		yajl_val v = yajl_tree_get(node, path, yajl_t_string);
		if (v)
			printf("%s/%s: %s\n", path[0], path[1], YAJL_GET_STRING(v));
		else
			printf("no such node: %s/%s\n", path[0], path[1]);
	}

	yajl_tree_free(node);

	return 0;
}

LIBSPIRIT_API int curltest(char* url) {
	CURL *curl;
	CURLcode res;
	struct curl_slist *slist = NULL;
	char agent[20];
	struct MemoryStruct chunk;

	chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */
	chunk.size = 0;    /* no data at this point */

	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, url);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);

#		ifdef SKIP_PEER_VERIFICATION
		/*
		 * If you want to connect to a site who isn't using a certificate that is
		 * signed by one of the certs in the CA bundle you have, you can skip the
		 * verification of the server's certificate. This makes the connection
		 * A LOT LESS SECURE.
		 *
		 * If you have a CA cert for the server stored someplace else than in the
		 * default bundle, then the CURLOPT_CAPATH option might come handy for
		 * you.
		 */
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#		endif

#		ifdef SKIP_HOSTNAME_VERIFICATION
		/*
		 * If the site you're connecting to uses a different host name that what
		 * they have mentioned in their server certificate's commonName (or
		 * subjectAltName) fields, libcurl will refuse to connect. You can skip
		 * this check, but this will make the connection less secure.
		 */
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#		endif

		curl_easy_setopt(curl, CURLOPT_SSL_CIPHER_LIST, "DHE-RSA-AES256-SHA");
		sprintf(agent, "libspirit/%d.%d", libspirit_VERSION_MAJOR,
				libspirit_VERSION_MINOR);
		curl_easy_setopt( curl, CURLOPT_USERAGENT, agent);

		slist = curl_slist_append(slist, "Accept: application/json");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);

		/* send all data to this function  */
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

		/* we pass our 'chunk' struct to the callback function */
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

		res = curl_easy_perform(curl);

		curl_slist_free_all(slist); /* free the list again */

		/* always cleanup */
		curl_easy_cleanup(curl);

		printf("res = %i -- %s\n", res, curl_easy_strerror(res));
		printf("%lu bytes retrieved\n", (long)chunk.size);
		printf("\n-- Data --\n\n");
		//printf(chunk.memory);

		/* parse it */
		{
			yajl_val node;
			char errbuf[1024];

			/* we have the whole config file in memory.  let's parse it ... */
			node = yajl_tree_parse((const char *) chunk.memory, errbuf,
					sizeof(errbuf));

			/* parse error handling */
			if (node == NULL) {
				fprintf(stderr, "parse_error: ");
				if (strlen(errbuf))
					fprintf(stderr, " %s", errbuf);
				else
					fprintf(stderr, "unknown error");
				fprintf(stderr, "\n");
				return 1;
			}

			/* ... and extract a nested value from the config file */
			{
				const char * path[] = { "news", (const char *) 0 };
				yajl_val v = yajl_tree_get(node, path, yajl_t_array);
				if (YAJL_IS_ARRAY(v)) {
					yajl_val *allNews = v->u.array.values;//YAJL_GET_ARRAY(v);
					unsigned int i;
					for (i = 0; i < v->u.array.len; ++i) {
						const char * path[] = { "title", (const char *) 0 };
						//printf("\n%X", allNews);

						yajl_val v = yajl_tree_get(*(allNews), path, yajl_t_string);

						if (YAJL_IS_STRING(v))
							printf("%s: %s\n", path[0], YAJL_GET_STRING(v));
						//else
						//	printf("no such node: %s\n", path[0]);

						++allNews;
					}



				}
				else
					printf("no such node: %s\n", path[0]);
			}

			yajl_tree_free(node);
		}


		if(chunk.memory)
		    free(chunk.memory);
	}
	return 0;
}

static SPIRITcode Spirit_initLibcurlSettings(struct LibcurlSettings *curl) {
	SPIRITcode res = SPIRITE_OK;
	//curl = malloc(sizeof(struct LibcurlSettings));
	curl->header_accept = "Accept: application/json";
	curl->ssl_cipher_type = "DHE-RSA-AES256-SHA";
	curl->user_agent = "libspirit/" STR_VALUE(libspirit_VERSION_MAJOR) "." STR_VALUE(libspirit_VERSION_MINOR);

	return res;
}

static SPIRITcode Spirit_initCurlConnectionForUrl(struct SpiritHandle *spirit, CURL **curl_handle, const char *url, struct MemoryStruct *chunk) {
	struct curl_slist *slist = NULL;
	char *request_url;//[strlen(spirit->base_url) + strlen(url) + 1];
	SPIRITcode res = SPIRITE_OK;
	CURL *curl;

	request_url = calloc(strlen(spirit->base_url) + strlen(url) + 1, sizeof(char));
	if (request_url == NULL)
		return SPIRITE_OUT_OF_MEMORY;

	strcpy(request_url, spirit->base_url);
	strcpy(&(request_url[strlen(request_url)]), url);

	printf("request_url: %s\n", request_url);

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
		printf("spirit->curl.ssl_cipher_type: %s\nspirit->curl.user_agent: %s\nspirit->curl.header_accept: %s\n", spirit->curl.ssl_cipher_type, spirit->curl.user_agent, spirit->curl.header_accept);
		slist = curl_slist_append(slist, spirit->curl.header_accept);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);

		/* send all data to this function  */
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

		/* we pass our 'chunk' struct to the callback function */
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)chunk);
	}
	*curl_handle = curl;

	printf("in setup: %X\n", &curl);

	return res;
}

LIBSPIRIT_API SPIRIT *spirit_init(const char *base_url) {
	struct SpiritHandle *handle;
	handle = calloc(1, sizeof(struct SpiritHandle));
	if (handle == NULL)
			return NULL;


	fprintf(stdout, "ein handle ist: %d Byte gross.\n", sizeof(struct SpiritHandle));

	Spirit_initLibcurlSettings(&handle->curl);

	handle->base_url = base_url;





	return handle;
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
	printf("preperform %X\n", &curl);
	curl_easy_perform(curl);
	printf("performed well\n");
	/* always cleanup -- did we cleanup the slist too? */
	curl_easy_cleanup(curl);

	printf("\n--- ALL NEWS ---\n");
	printf(chunk.memory);
	printf("\n================\n");

	return res;
}


