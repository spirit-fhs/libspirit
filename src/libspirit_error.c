/*
 * libspirit_error.c
 *
 *  Created on: 10.08.2011
 *      Author: delbertooo
 */

#include <libspirit/spirit.h>

LIBSPIRIT_API const char *spirit_errstring(SPIRITcode error) {
	switch (error) {
	case SPIRITE_OK:
		return "No error";

	case SPIRITE_OUT_OF_MEMORY:
		return "Out of memory";

	case SPIRITE_JSON_PARSE_ERROR:
		return "Error while parsing the JSON string";

	case SPIRITE_JSON_NODE_NOT_FOUND:
		return "Didn't found node not in JSON tree";

		/* error codes not used by current libspirit */
	case SPIRITE_OBSOLETE2:
	case SPIRITE_OBSOLETE3:
	case SPIRIT_LAST:
		break;
	}

	return "Unknown error";
}
