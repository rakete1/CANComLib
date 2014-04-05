 /*
    CANComTest - A program to test the CANComLib
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
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#include "../src/cancom.h"

/**
 * This program loads and links the CANComLib dynamically to send a CAN test frame to the socket 
 */
int main(int argc, char **argv)
{
   void *lib_handle;
   void (*fn)(void);
   int x;
   char *error;

   printf("starting testlib...\n");

   lib_handle = dlopen("/system/lib/libcancom.so", /*RTLD_LAZY*/ RTLD_NOW);
   if (!lib_handle)
   {
	  printf("%s\n", dlerror());
      exit(1);
   }

   fn = dlsym(lib_handle, "send_can_testframe");
   if ((error = dlerror()) != NULL)
   {
      printf("%s\n", error);
      exit(1);
   }

   printf("sending CAN-Message...\n");
   (*fn)();
   printf("done!\n");

   dlclose(lib_handle);

   exit(0);
   return 0;
}
