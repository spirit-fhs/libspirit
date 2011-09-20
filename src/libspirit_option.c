/*
 * libspirit_option.c
 *
 *  Created on: 12.08.2011
 *      Author: delbertooo
 */
#include <stdarg.h>
#include <libspirit/spirit.h>
#include "utility.h"
#include "libspirit_data.h"
#include "libspirit_option.h"

/* sets a spirit option to a value */
#undef spirit_setopt
LIBSPIRIT_API SPIRITcode spirit_setopt(SPIRIT *handle, SPIRIToption tag, ...)
{
  va_list arg;
  struct SpiritHandle *data = handle;
  SPIRITcode ret;

  if(!handle)
    return SPIRITE_BAD_FUNCTION_ARGUMENT;

  va_start(arg, tag);

  ret = Spirit_setopt(data, tag, arg);

  va_end(arg);
  return ret;
}

/* sets a string-option to a given value */
static SPIRITcode setstropt(char **charp, char * s)
{
	if (*charp) {
		free(*charp);
		*charp = (char *) NULL;
	}

	if (s) {
		s = my_strdup(s);

		if (!s)
			return SPIRITE_OUT_OF_MEMORY;

		*charp = s;
	}

	return SPIRITE_OK;
}

/* internal function to set an option */
SPIRITcode Spirit_setopt(struct SpiritHandle *data, SPIRIToption option,
		va_list param)
{
	/*char *argptr;*/
	SPIRITcode result = SPIRITE_OK;

	/* Usage exaples:
	 *
	 *   bool
	 *       long use_cache = va_arg(param, long);
	 *       data.my_val = (bool) (0 != use_cache);
	 *   long
	 *       data.my_val = va_arg(param, long);
	 *   string
	 *       result = setstropt(&data->my_string, va_arg(param, char *));
	 */

	switch (option) {
	case SPIRITOPT_SSL_SIPHER_TYPE:
		result = setstropt(&data->curl.ssl_cipher_type, va_arg(param, char *));
		break;
	case SPIRITOPT_HEADER_ACCEPT:
		result = setstropt(&data->curl.header_accept, va_arg(param, char *));
		break;
	case SPIRITOPT_BASE_URL:
		result = setstropt(&data->base_url, va_arg(param, char *));
		break;

	default:
		/* unknown tag and its companion, just ignore: */
		result = SPIRITE_UNKNOWN_OPTION;
		break;
	}

	return result;
}
