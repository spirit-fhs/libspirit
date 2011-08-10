/*
 * spirit_data.h
 *
 *  Created on: 10.08.2011
 *      Author: delbertooo
 */

#ifndef SPIRIT_DATA_H_
#define SPIRIT_DATA_H_

struct SpiritHandle {
	struct LibcurlSettings *curl;
};

struct LibcurlSettings {
	char *ssl_cipher_type;
	char *header_accept;
	char *user_agent;
};


#endif /* SPIRIT_DATA_H_ */
