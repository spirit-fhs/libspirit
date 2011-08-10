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

	spirit_handle = spirit_init();


	printf("-- libspirit test application --\n");
	fflush(stdout);

	curltest("https://212.201.64.226:8443/fhs-spirit/news");

	return EXIT_SUCCESS;
}
