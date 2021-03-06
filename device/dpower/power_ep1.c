/*-------------------------------------------------------------------------
  power_ep1.c - USB endpoint 1 callbacks

             (c) 2006 Pierre Gaufillet <pierre.gaufillet@magic.fr> 

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
-------------------------------------------------------------------------*/

/* $Id$ */

#include "power_ep1.h"
#include "power_ep2.h"
#include "power_mgr.h"
#include "usb.h"
#include "application_debug.h"
#include <pic18fregs.h>
#include <delay.h>


#pragma udata usb_buf power_ep1_OutBuffer
volatile uchar power_ep1_OutBuffer[POWER_EP1_BUFFER_SIZE];

void power_ep1_init(void)
{
    debug("power_ep1_init\n");
    EP_OUT_BD(1).Cnt = POWER_EP1_BUFFER_SIZE;
    EP_OUT_BD(1).ADR = (uchar __data *)&power_ep1_OutBuffer;
    EP_OUT_BD(1).Stat.uc = BDS_USIE | BDS_DAT0 | BDS_DTSEN;
    UEP1 = EPHSHK_EN | EPOUTEN_EN | EPCONDIS_EN;       // Init EP1 as an OUT EP
}

void power_ep1_out(void)
{
    if(EP_OUT_BD(1).Cnt >= 1)
    {
        switch(power_ep1_OutBuffer[0])
        {
            case READ_BATTERY_TENSION_CMD:
                debug("READ_BATTERY_TENSION_CMD\n");
                start_read_tension();
                break;
            case POWER_SUPPLY_ON_CMD:
                debug("POWER_SUPPLY_ON_CMD\n");
                switch_power_supply_on(power_ep1_OutBuffer[1] + 256 * power_ep1_OutBuffer[2]);
                break;
            case RESET_POWER_SUPPLY_CMD:
                debug("RESET_POWER_SUPPLY_CMD\n");
                reset_power_supply(power_ep1_OutBuffer[1] + 256 * power_ep1_OutBuffer[2]);
                break;
            case SHUTDOWN_POWER_SUPPLY_CMD:
                debug("SHUTDOWN_POWER_SUPPLY_CMD\n");
                shutdown_power_supply(power_ep1_OutBuffer[1] + 256 * power_ep1_OutBuffer[2]);
                break;
            case SET_BATTERY_TENSION_ALERT_CMD:
                debug("SET_BATTERY_TENSION_ALERT_CMD\n");
                set_battery_tension_alert(power_ep1_OutBuffer[1] + 256 * power_ep1_OutBuffer[2], 
                                          (power_ep1_OutBuffer[3] + 256 * power_ep1_OutBuffer[4]) & 0x3ff);
                break;
            case UNSET_BATTERY_TENSION_ALERT_CMD:
                debug("UNSET_BATTERY_TENSION_ALERT_CMD\n");
                unset_battery_tension_alert();
                break;
                
            default:
                debug("Unknown power management command\n");
                // Raise an error
                EP_OUT_BD(1).Cnt = POWER_EP1_BUFFER_SIZE;
                EP_OUT_BD(1).ADR = (uchar __data *)&power_ep1_OutBuffer;
                EP_OUT_BD(1).Stat.uc = BDS_USIE | BDS_BSTALL;
                break;
        }
        EP_OUT_BD(1).Cnt = POWER_EP1_BUFFER_SIZE;
        if(EP_OUT_BD(1).Stat.DTS == 0)
        {
            EP_OUT_BD(1).Stat.uc = BDS_USIE | BDS_DAT1 | BDS_DTSEN;
        }
        else
        {
            EP_OUT_BD(1).Stat.uc = BDS_USIE | BDS_DAT0 | BDS_DTSEN;
        }
    }
    else // Raise an error
    {
        EP_OUT_BD(1).Cnt = POWER_EP1_BUFFER_SIZE;
        EP_OUT_BD(1).ADR = (uchar __data *)&power_ep1_OutBuffer;
        EP_OUT_BD(1).Stat.uc = BDS_USIE | BDS_BSTALL;
    }
}
