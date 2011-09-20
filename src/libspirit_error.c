/*
 * libspirit_error.c
 *
 *  Created on: 10.08.2011
 *      Author: delbertooo
 */

#include <libspirit/spirit.h>

LIBSPIRIT_API const char *spirit_errstring(SPIRITcode error)
{
	switch (error) {
	case SPIRITE_OK:
		return "No error";

	case SPIRITE_OUT_OF_MEMORY:
		return "Out of memory";

	case SPIRITE_JSON_PARSE_ERROR:
		return "Error while parsing the JSON string";

	case SPIRITE_JSON_NODE_NOT_FOUND:
		return "Didn't found node not in JSON tree";

	case SPIRITE_BAD_FUNCTION_ARGUMENT:
		return "Bad function argument given";

	case SPIRITE_UNKNOWN_OPTION:
		return "Unknown option given";

		/* error codes not used by current libspirit */
	case SPIRITE_OBSOLETE2:
	case SPIRITE_OBSOLETE3:
	case SPIRITE_OBSOLETE6:
	case SPIRITE_OBSOLETE7:
	case SPIRITE_OBSOLETE8:
	case SPIRIT_LAST:
		break;
	}

	return "Unknown error";
}
