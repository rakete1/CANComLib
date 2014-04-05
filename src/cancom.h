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

#ifndef CANCOM_H_
#define CANCOM_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <net/if.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>

#include <linux/can/raw.h>
#include <linux/can.h>
#include <linux/socket.h>

/* Maybe PF_CAN and AF_CAN are not defined in the kernel headers */
#ifndef PF_CAN
#define PF_CAN 29
#endif
#ifndef AF_CAN
#define AF_CAN PF_CAN
#endif

#define CAN_INTERFACE 	"can0"

#ifdef __cplusplus
extern "C" {
#endif

/* Storage of the global socket */
int global_socket = -1;

/* External interface functions of the library. These can be linked dynamically by dlopen, dlsym, etc. */
extern int initialize();
extern int terminate();
extern struct can_frame receive_can(struct can_filter filter);
extern int send_can(struct can_frame sendFrame);
extern void send_can_testframe();

/* Local helper functions */
int init_cansocket(const char *ifname);
int sendframe(int skt, struct can_frame sendFrame);
struct can_frame readframe(int skt, struct can_filter filters[]);

#ifdef __cplusplus
}
#endif

#endif /* CANCOM_H_ */
