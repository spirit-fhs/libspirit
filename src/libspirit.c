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
#include "utility.h"

#define SKIP_PEER_VERIFICATION DEFINED
#define SKIP_HOSTNAME_VERIFICATION DEFINED





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


SPIRITcode Spirit_initLibcurlSettings(struct LibcurlSettings *curl) {
	SPIRITcode res = SPIRITE_OK;

	curl->header_accept = "Accept: application/json";
	curl->ssl_cipher_type = "DHE-RSA-AES256-SHA";
	curl->user_agent = libspirit_USER_AGENT;

	return res;
}

SPIRITcode Spirit_initCurlConnectionForUrl(struct SpiritHandle *spirit, CURL **curl_handle, const char *url, struct MemoryStruct *chunk) {
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




