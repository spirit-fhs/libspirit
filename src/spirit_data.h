/*
 * spirit_data.h
 *
 *  Created on: 10.08.2011
 *      Author: delbertooo
 */

#ifndef SPIRIT_DATA_H_
#define SPIRIT_DATA_H_

#include <libspirit/libspirit.h>

struct LibcurlSettings {
	char *ssl_cipher_type;
	char *header_accept;
	char *user_agent;
};

struct SpiritHandle {
	struct LibcurlSettings curl;
	char *base_url;
};




#endif /* SPIRIT_DATA_H_ */
