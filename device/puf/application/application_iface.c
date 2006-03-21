/*-------------------------------------------------------------------------
  application_iface.c - Application data

             (c) 2006 Pierre Gaufillet <pierre.gaufillet@magic.fr> 

   This library is free software; you can redistribute it and/or modify it
   under the terms of the GNU Library General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.
   
   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Library General Public License for more details.
   
   You should have received a copy of the GNU Library General Public License
   along with this program; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
-------------------------------------------------------------------------*/

#include "common_types.h"
#include "usb_descriptors.h"

void application_main(void);

typedef struct {
    uchar invalid;                   // != 0 when the application is not valid
    void* device_descriptor;
    void** configuration_descriptor; // pointer to an array of pointer to configuration descriptors
    uchar** string_descriptor;       // pointer to an array of pointer to string descriptors
    void (*** ep_init)(void);
    void (*** ep_in)(void);
    void (*** ep_out)(void);
    void (*** ep_setup)(void);
    void (*main) (void);
} ApplicationData;

const ApplicationData __at(0x2000) application_data = {
    0x00,                          // Application is valid
    &device_descriptor,            // device descriptor
    configuration_descriptor,      // configuration descriptor
    string_descriptor,             // string descriptor
    ep_init,                       // ep_init
    ep_in,                         // ep_in
    ep_out,                        // ep_out
    ep_setup,                      // ep_setup
    application_main               // application main
};
