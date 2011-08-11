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
#include <libspirit/libspirit_error.h>

int main(void) {
	SPIRIT *spirit_handle;
	SPIRITcode res;

	printf("-- libspirit test application --\n");

	spirit_handle = spirit_init("https://212.201.64.226:8443/fhs-spirit/");
	printf("\n=================================== ALL NEWS ===================================\n");
	res = spirit_news_print_all(spirit_handle);
	printf("\n================================================================================\n");
	printf("\nspirit result %i: %s\n", res, spirit_errstring(res));

	spirit_cleanup(spirit_handle);

	//printf("-- libspirit test application --\n");
	//fflush(stdout);

	//curltest("https://212.201.64.226:8443/fhs-spirit/news");

	return EXIT_SUCCESS;
}
