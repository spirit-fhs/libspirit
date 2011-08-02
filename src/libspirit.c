/*
 * libspirit.c
 *
 *  Created on: 01.08.2011
 *      Author: delbertooo
 */
#include "libspirit.h"
#include "libspiritconfig.h"
#include "yajl/yajl_gen.h"
#include "yajl/yajl_parse.h"


LIBSPIRIT_API int fndlltest(void)
{
	yajl_gen g;
	yajl_status st;
	yajl_handle h;
	st = yajl_parse(h, "[]", 3);
	return 42;
}
