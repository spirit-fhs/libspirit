/*
 * spirit.h
 *
 *  Created on: 04.08.2011
 *      Author: delbertooo
 */

#ifndef SPIRIT_H_
#define SPIRIT_H_

#ifdef __cplusplus
extern "C" {
#endif

#if (defined(_WIN32) || defined(WIN32))
	#ifdef libspirit_EXPORTS
		#define LIBSPIRIT_API __declspec(dllexport)
	#else
		#define LIBSPIRIT_API __declspec(dllimport)
	#endif
#else /* not MSVC */
	#define LIBSPIRIT_API
#endif


#define SPIRITOPTTYPE_LONG          0
#define SPIRITOPTTYPE_OBJECTPOINT   10000
#define SPIRITOPTTYPE_FUNCTIONPOINT 20000
#define SPIRITOPTTYPE_OFF_T         30000

#ifdef SINIT
#undef SINIT
#endif

#define SINIT(name,type,number) SPIRITOPT_ ## name = SPIRITOPTTYPE_ ## type + number
typedef enum {
	/* base url for REST request */
	SINIT(BASE_URL, OBJECTPOINT, 1),

	/* sipher type of ssl (advanced option) */
	SINIT(SSL_SIPHER_TYPE,  OBJECTPOINT, 2),

	/* header accept, like "Accept: application/json" */
	SINIT(HEADER_ACCEPT, OBJECTPOINT, 3),


	SPIRITOPT_LASTENTRY /* the last unused */
} SPIRIToption;

LIBSPIRIT_API typedef enum {
  SPIRITE_OK = 0,
  SPIRITE_OUT_OF_MEMORY,         /* 1  - memory allocation failed */
  SPIRITE_OBSOLETE2,             /* 2  - NOT USED */
  SPIRITE_OBSOLETE3,             /* 3  - NOT USED */
  SPIRITE_JSON_PARSE_ERROR,      /* 4  - error while parsing the JSON string */
  SPIRITE_JSON_NODE_NOT_FOUND,   /* 5  - node not in JSON tree */
  SPIRITE_OBSOLETE6,             /* 6  - NOT USED */
  SPIRITE_OBSOLETE7,             /* 7  - NOT USED */
  SPIRITE_OBSOLETE8,             /* 8  - NOT USED */
  SPIRITE_BAD_FUNCTION_ARGUMENT, /* 9  - bad function argument given */
  SPIRITE_UNKNOWN_OPTION,        /* 10 - unknown option given */


  SPIRIT_LAST /* never use! */
} SPIRITcode;

LIBSPIRIT_API typedef void SPIRIT;


LIBSPIRIT_API SPIRIT *spirit_init(const char *base_url);
LIBSPIRIT_API void spirit_cleanup(SPIRIT *handle);
LIBSPIRIT_API SPIRITcode spirit_setopt(SPIRIT *handle, SPIRIToption tag, ...);

/* sets the call to exact three params (gives compiler errors for oblivious people) */
#define spirit_setopt(handle,opt,param) spirit_setopt(handle,opt,param)

#ifdef __cplusplus
}
#endif


#endif /* SPIRIT_H_ */
