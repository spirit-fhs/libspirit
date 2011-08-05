/*
 * libspirit.c
 *
 *  Created on: 01.08.2011
 *      Author: delbertooo
 */

#include <stdio.h>
#include <string.h>
#include <stddef.h>

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
	yajl_config(h, yajl_allow_comments, 1);
	st = yajl_parse(h, "{}", 2);
	return 42;
}

LIBSPIRIT_API int curltest(char* url)
{
	CURL *curl;
	CURLcode res;

	curl = curl_easy_init();
	if(curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, url);
		res = curl_easy_perform(curl);

		printf("res = %i -- %s\n", res, curl_easy_strerror(res));

		/* always cleanup */
		curl_easy_cleanup(curl);
	}
	return 0;
}

/* Auxiliary function that waits on the socket. */
int wait_on_socket(curl_socket_t sockfd, int for_recv, long timeout_ms)
{
  struct timeval tv;
  fd_set infd, outfd, errfd;
  int res;

  tv.tv_sec = timeout_ms / 1000;
  tv.tv_usec= (timeout_ms % 1000) * 1000;

  FD_ZERO(&infd);
  FD_ZERO(&outfd);
  FD_ZERO(&errfd);

  FD_SET(sockfd, &errfd); /* always check for error */

  if(for_recv)
  {
    FD_SET(sockfd, &infd);
  }
  else
  {
    FD_SET(sockfd, &outfd);
  }

  /* select() returns the number of signalled sockets or -1 */
  res = select(sockfd + 1, &infd, &outfd, &errfd, &tv);
  return res;
}

LIBSPIRIT_API int curltest2(void) {
	CURL *curl;
	CURLcode res;
	/* Minimalistic http request */
	const char *request = "GET / HTTP/1.0\r\nHost: google.de\r\n\r\n";
	curl_socket_t sockfd; /* socket */
	long sockextr;
	size_t iolen;

	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "http://google.de");
		/* Do not do the transfer - only connect to host */
		curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
		res = curl_easy_perform(curl);

		if (CURLE_OK != res) {
			printf("Error1: %s\n", strerror(res));
			return 1;
		}

		/* Extract the socket from the curl handle - we'll need it for waiting.
		 * Note that this API takes a pointer to a 'long' while we use
		 * curl_socket_t for sockets otherwise.
		 */
		res = curl_easy_getinfo(curl, CURLINFO_LASTSOCKET, &sockextr);

		if (CURLE_OK != res) {
			printf("Error2: %s\n", curl_easy_strerror(res));
			return 1;
		}

		sockfd = sockextr;

		/* wait for the socket to become ready for sending */
		if (!wait_on_socket(sockfd, 0, 60000L)) {
			printf("Error3: timeout.\n");
			return 1;
		}

		puts("Sending request.");
		/* Send the request. Real applications should check the iolen
		 * to see if all the request has been sent */
		res = curl_easy_send(curl, request, strlen(request), &iolen);

		if (CURLE_OK != res) {
			printf("Erro4r: %s\n", curl_easy_strerror(res));
			return 1;
		}
		puts("Reading response.");

		/* read the response */
		for (;;) {
			char buf[1024];

			wait_on_socket(sockfd, 1, 60000L);
			res = curl_easy_recv(curl, buf, 1024, &iolen);

			if (CURLE_OK != res)
				break;

			printf("Received %u bytes.\n", iolen);
		}

		/* always cleanup */
		curl_easy_cleanup(curl);
	}
	return 0;
}
