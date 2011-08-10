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
		sprintf(agent, "libspirit/%i.%i", libspirit_VERSION_MAJOR,
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
					int i;
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

LIBSPIRIT_API SPIRIT *spirit_init(void) {
	struct SpiritHandle *handle;

	return handle;
}
