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
#include <libspirit/spirit_news.h>
#include <libspirit/spirit_error.h>
#include <libspirit/spirit_schedule.h>

int main(void) {
	SPIRIT *spirit_handle;
	SPIRIT *schedule_handle;
	SPIRITcode res;
	int sz = 0;


	printf("-- libspirit test application --\n");

	spirit_handle = spirit_init("https://spirit.fh-schmalkalden.de/");
	printf("\n=================================== ALL NEWS ===================================\n");
	res = spirit_news_print_all(spirit_handle);
	printf("\n================================================================================\n");
	printf("\n\tSPIRIT RESULT %i: %s\n", res, spirit_errstring(res));

	printf("\n\n");
	//spirit_setopt(spirit_handle, SPIRITOPT_HEADER_ACCEPT, "Accept: application/xml");
	//res = spirit_news_print_all(spirit_handle);
	//printf("\n\tSPIRIT RESULT (no more error expected) %i: %s\n", res, spirit_errstring(res));

	/* test of the failing stuff */
	/*res = spirit_news_by_date(spirit_handle, NULL, &sz);
	printf("\n\tSPIRIT RESULT NEW %i: %s\n", res, spirit_errstring(res));
	printf("groesse: %d\n", sz);

	if (res == SPIRITE_OK) {
		char *mem = malloc(sz);
		if (!mem)
			return EXIT_FAILURE;
		res = spirit_news_by_date(spirit_handle, &mem, &sz);
		printf("\n\tSPIRIT RESULT NEW MEM %i: %s\n", res, spirit_errstring(res));
		printf(mem);
	}*/
	schedule_handle = spirit_init("https://spirit.fh-schmalkalden.de/rest/");

	spirit_schedule_print(schedule_handle, "MaI1", 'g');

	spirit_cleanup(schedule_handle);
	spirit_cleanup(spirit_handle);

	return EXIT_SUCCESS;
}
