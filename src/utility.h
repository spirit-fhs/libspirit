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

#ifdef __cplusplus
extern "C" {
#endif

struct MemoryStruct {
	char *memory;
	size_t size;
};

void fprintNChars(FILE *file, char c, unsigned int n);
char *my_strdup(const char *s);

#ifdef __cplusplus
}
#endif

#endif /* UTILITY_H_ */
