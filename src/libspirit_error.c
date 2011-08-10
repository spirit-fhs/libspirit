/*
 * libspirit_error.c
 *
 *  Created on: 10.08.2011
 *      Author: delbertooo
 */

#include <libspirit/libspirit.h>

LIBSPIRIT_API const char *spirit_errstring(SPIRITcode error) {
	switch (error) {
	case SPIRITE_OK:
		return "No error";

	case SPIRITE_OUT_OF_MEMORY:
		return "Out of memory";

		/* error codes not used by current libspirit */
	case SPIRITE_OBSOLETE2:
	case SPIRIT_LAST:
		break;
	}

	return "Unknown error";
}
