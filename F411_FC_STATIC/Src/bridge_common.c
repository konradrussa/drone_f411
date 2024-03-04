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

const char zero = '0';
const char space = ' ';
const char semicolon = ';';

static uint32_t radio_commands[6];
const int radio_channels_number = sizeof(radio_commands)
		/ sizeof(radio_commands[0]);

static uint32_t motor_commands[7];
const int motors_number = sizeof(motor_commands) / sizeof(motor_commands[0]);

static char sequence[5];
const int sequence_length = sizeof(sequence) / sizeof(sequence[0]);

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
// free(fc_bridge.port->data_in);  // deallocate memory
// fc_bridge.port->data_in = NULL; // set NULL
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

void bridge_fill_sequence(char *data) {
	const int seq_len_with_zero = bridge_get_sequence_length() - 1;
	sequence[seq_len_with_zero] = *bridge_get_semicolon();
	int data_length = strlen(data);
	for (int i = 0; i < seq_len_with_zero - data_length; i++) {
		sequence[i] = zero;
	}
	int j = data_length;
	for (int i = seq_len_with_zero; i > seq_len_with_zero - data_length; i--) {
		sequence[i - 1] = data[j - 1];
		j--;
	}
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

inline const char* bridge_get_sequence() {
	return sequence;
}

inline const int bridge_get_sequence_length() {
	return sequence_length;
}

inline uint32_t* bridge_get_motor_commands() {
	return motor_commands;
}

inline const int bridge_get_motors_number() {
	return motors_number;
}

inline uint32_t* bridge_get_radio_commands() {
	return radio_commands;
}

inline const int bridge_get_radio_channels_number() {
	return radio_channels_number;
}
