/*
 ============================================================================
 Name        : testapp.c
 Author      : Robert Worgul
 Version     :
 Copyright   : 2011
 Description : libspirit test application
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <libspirit/libspirit.h>

int main(void) {
	SPIRIT *spirit_handle;
	SPIRITcode res;

	spirit_handle = spirit_init("https://212.201.64.226:8443/fhs-spirit/");
	res = spirit_news_print_all(spirit_handle);

	printf("\nspirit result %d: %s\n", res, spirit_errstring(res));

	//printf("-- libspirit test application --\n");
	//fflush(stdout);

	//curltest("https://212.201.64.226:8443/fhs-spirit/news");

	return EXIT_SUCCESS;
}
