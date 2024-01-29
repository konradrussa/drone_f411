/*
 * bridge_common.c
 *
 *  Created on: Jan 10, 2024
 *      Author: konrad
 */
#include "bridge_common.h"

//bridge signals
const short FC_BRIDGE_TRANSMIT_SIGINT = 300;
const short FC_BRIDGE_RECEIVE_SIGINT = 301;

const char space = ' ';
const char semicolon = ';';

// "1097;1919;1508;1508;1919;"
bool bridge_validate_data(char *s, int size) {
	int length = strlen(s);
	if (s == NULL || length < size) {
		return false;
	}
	return true;
}

// fc_bridge.port->data_in = bridge_format_string((char*) fc_bridge.rx_data); // allocate memory
// process data
// free(fc_bridge.port->data_in); // deallocate memory
char* bridge_format_string(char *s) {
	char *d = (char*) malloc(strlen(s));
	int counter = 0, space_num = 0;
	while (*s++) {
		if (*s != semicolon && isdigit((int )*s)) {
			if (counter != 0 && (counter - space_num) % 4 == 0) {
				d[counter++] = space;
				space_num++;
			}
			d[counter++] = *s;
		}
	}
	d[counter] = (char) 0; //'\0'
	return d;
}

inline const short bridge_get_transmit_sigint() {
	return FC_BRIDGE_TRANSMIT_SIGINT;
}

inline const short bridge_get_receive_sigint() {
	return FC_BRIDGE_RECEIVE_SIGINT;
}

inline const char* bridge_get_space() {
	return &space;
}

inline const char* bridge_get_semicolon() {
	return &semicolon;
}
