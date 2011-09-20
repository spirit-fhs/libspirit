/*
 * utility.c
 *
 *  Created on: 11.08.2011
 *      Author: delbertooo
 */
#include <stdio.h>
#include <string.h>
#include "utility.h"

/* prints 'n' times the char 'c' to 'file' */
void fprintNChars(FILE *file, char c, unsigned int n) {
	unsigned int i;
	for (i = 0; i < n; ++i)
		fprintf(file, "%c", c);
}


/* function to duplicate a string in memory */
/*   - not yet implemented in C89/C90       */
char *my_strdup(const char *s) {
	char *p = malloc(strlen(s) + 1);
	if (p) {
		strcpy(p, s);
	}
	return p;
}

