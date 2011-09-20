/*
 * utility.h
 *
 *  Created on: 11.08.2011
 *      Author: delbertooo
 */

#ifndef UTILITY_H_
#define UTILITY_H_

#include <stdio.h>
#include <string.h>

#ifndef my_free
#define my_free(x) if (x != NULL) free(x);
#endif

struct MemoryStruct {
	char *memory;
	size_t size;
};

void fprintNChars(FILE *file, char c, unsigned int n);
char *my_strdup(const char *s);


#endif /* UTILITY_H_ */
