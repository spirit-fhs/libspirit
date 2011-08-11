/*
 * spirit.h
 *
 *  Created on: 04.08.2011
 *      Author: delbertooo
 */

#include <libspirit/spirit_common.h>

#ifndef SPIRIT_H_
#define SPIRIT_H_

#ifdef __cplusplus
extern "C" {
#endif



LIBSPIRIT_API typedef enum {
  SPIRITE_OK = 0,
  SPIRITE_OUT_OF_MEMORY,         /* 1  - memory allocation failed */
  SPIRITE_OBSOLETE2,             /* 2  - NOT USED */
  SPIRITE_OBSOLETE3,             /* 3  - NOT USED */
  SPIRITE_JSON_PARSE_ERROR,      /* 4  - error while parsing the JSON string */
  SPIRITE_JSON_NODE_NOT_FOUND,   /* 5  - node not in JSON tree */


  SPIRIT_LAST /* never use! */
} SPIRITcode;

LIBSPIRIT_API typedef void SPIRIT;


LIBSPIRIT_API SPIRIT *spirit_init(const char *base_url);
LIBSPIRIT_API void spirit_cleanup(SPIRIT *handle);




#ifdef __cplusplus
}
#endif


#endif /* SPIRIT_H_ */
