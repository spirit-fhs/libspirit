/*
 * libspirit_data.h
 *
 *  Created on: 10.08.2011
 *      Author: delbertooo
 */

#ifndef LIBSPIRIT_DATA_H_
#define LIBSPIRIT_DATA_H_

#include <libspirit/spirit.h>

struct LibcurlSettings {
	char *ssl_cipher_type;
	char *header_accept;
	char *user_agent;
};

struct SpiritHandle {
	struct LibcurlSettings curl;
	char *base_url;
};


#endif /* LIBSPIRIT_DATA_H_ */
