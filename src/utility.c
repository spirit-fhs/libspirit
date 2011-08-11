/*
 * utility.c
 *
 *  Created on: 11.08.2011
 *      Author: delbertooo
 */

#include "utility.h"

void fprintNChars(FILE *file, char c, unsigned int n) {
	unsigned int i;
	for (i = 0; i < n; ++i)
		fprintf(file, "%c", c);
}

char *my_strdup(const char *s) {
	char *p = malloc(strlen(s) + 1);
	if (p) {
		strcpy(p, s);
	}
	return p;
}

