/*-------------------------------------------------------------------------
  usbdevice.h - docker USB accesses

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

#ifndef USBDEVICE_H
#define USBDEVICE_H

using namespace std;

#include <iostream>
#include <sstream>
#include <vector>
#include <exception>
#include <usb.h>
#include "config.h"
#include "DataBuffer.h"

#define ERASE_FLASH_CMD        0
#define WRITE_FLASH_CMD        1
#define READ_FLASH_CMD         2
#define VALID_APPLICATION_CMD  3
#define RESET_CMD              4
#define SECTION_DESCRIPTOR_CMD 5

/* Sections attributes */
#define FLASH_SECTION_READ     0x01
#define FLASH_SECTION_WRITE    0x02


#define SECTION_DESCRIPTOR_SIZE 7

class USBDevice
{
    public:
        USBDevice(struct usb_bus *busses, long vendor, long product);
        ~USBDevice();
        void erase(void);
        void write(DataBuffer& tab);
        void read(DataBuffer& tab);
        void reset(void);
        void validate_application(void);    
    
    private:
        void erase_block(long address);
        void write_block(long address, DataBuffer& tab);
        void read_block(long address, DataBuffer& tab);
        void get_section_descriptors(void);

        struct usb_dev_handle* dh;

        class Range {
        public:
            Range() {};
            Range(unsigned char f, long b, long e) : flags(f),begin(b),end(e) {};
            ~Range() {};
            long begin;
            long end;
            unsigned char flags;
        };
        
    public:
        typedef vector<Range*> RangeVector;
        
        RangeVector device_mmap;
        
    };

    
#endif // USBDEVICE_H
