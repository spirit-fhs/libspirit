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

/* simple macro to free memory if not NULL */
#ifdef SPIRIT_FREE
#undef SPIRIT_FREE
#endif
#define SPIRIT_FREE(x) if (x != NULL) free(x);

/* a struct to hold a chunk of memory */
struct MemoryStruct {
	char *memory;
	size_t size;
};

void fprintNChars(FILE *file, char c, unsigned int n);
char *my_strdup(const char *s);


#endif /* UTILITY_H_ */
