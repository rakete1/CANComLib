 /*
    CANComLib - A shared library that can be used to send and receive CAN frames
    Copyright (C) 2011  Ralph Rakers <ralph.rakers@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>
*/

#include "cancom.h"

/**
 * Initializes the global CAN socket. Must be called before receive_can and send_can.
 */
int initialize() {
	global_socket = init_cansocket(CAN_INTERFACE);
	return global_socket;
}

/**
 * Closes the global CAN socket. After the socket is closed calls to receive_can and send_can will return an error.
 */
int terminate() {
	if (global_socket >= 0) {
		return close(global_socket);
	}
	return -1;
}

/**
 * Returns the first CAN frame for the given filter
 */
struct can_frame receive_can(struct can_filter filter)
{
	return readframe(global_socket, &filter);
}

/**
 * Sends the given CAN frame
 */
int send_can(struct can_frame sendFrame)
{
	return sendframe(global_socket, sendFrame);
}

/**
 * Sends a CAN testframe to the socket. This function is used by the supplied testprogram in "testlib/main.c"
 */
void send_can_testframe() {
	struct can_frame sendFrame;

	sendFrame.can_id = 0x123;
	sendFrame.can_dlc = 1;
	sendFrame.data[0] = 0x22;

	sendframe(global_socket, sendFrame);
}

/**
 * Initializes the CAN socket for the given interface name.
 */
int init_cansocket(const char *ifname){
	int skt;
	struct sockaddr_can addr;
	struct ifreq ifr;

	/* Create the socket */
	if ((skt = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0){
		return -1;
	}

	/* Locate the interface we wish to use */
	strcpy(ifr.ifr_name, ifname);
	if ((ioctl(skt, SIOCGIFINDEX, &ifr)) < 0){
		return -1;
	}
	
	/* Select that CAN interface, and bind the socket to it. */
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;
	if (bind(skt, (struct sockaddr *)&addr, sizeof(addr)) < 0){
		return -1;
	}
	
	/* CAN socket initialization successful*/
	return skt;
}

/**
 * Helper function for sending a CAN frame to the given socket.
 * Return values: 0 = send successful, -1 = send failed
 */
int sendframe(int skt, struct can_frame sendFrame)
{
	if (write(skt, &sendFrame, sizeof(struct can_frame)) != sizeof(struct can_frame)){	
		return -1;
	}
	return 0;
}

/**
 * Helper function for reading a CAN frame from the given socket, which fits to the given filters
 */
struct can_frame readframe(int skt, struct can_filter filters[]){
	struct can_frame recvFrame;
	
	setsockopt(skt, SOL_CAN_RAW, CAN_RAW_FILTER, &filters, sizeof(filters));

	if(read(skt, &recvFrame, sizeof(struct can_frame)) < 0) {
		recvFrame.can_id = CAN_ERR_FLAG;
	}

	return recvFrame;
}

