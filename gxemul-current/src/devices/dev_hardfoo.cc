/*
 *  Copyright (C) 2007-2009  Anders Gavare.  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright  
 *     notice, this list of conditions and the following disclaimer in the 
 *     documentation and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 *  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE   
 *  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 *  OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 *  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 *  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 *  SUCH DAMAGE.
 *   
 *
 *  COMMENT: Realtek 8139 ethernet controller
 *
 *  TODO: Pretty much everything.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpu.h"
#include "device.h"
#include "emul.h"
#include "interrupt.h"
#include "machine.h"
#include "memory.h"
#include "misc.h"
#include "net.h"

#define	DEV_HARDFOO_LENGTH	0x100

struct hardfoo_data {

	/*  Registers:  */
	uint8_t			rl_command;
	uint8_t			rl_eecmd;

};


DEVICE_ACCESS(hardfoo)
{
	//struct hardfoo_data *d = (struct rtl8139c_data *) extra;
	uint64_t idata = 0, odata = 0;

	if (writeflag == MEM_WRITE)
		idata = memory_readmax64(cpu, data, len);

	switch (relative_addr) {

	case 0x1:
		if (writeflag == MEM_WRITE) {
			//if (idata & RL_CMD_RESET) {
				/*  Reset. TODO  */

				/*  ... and then clear the reset bit:  */
			//	idata &= ~RL_CMD_RESET;
			//}

			//d->rl_command = idata;
		} else {
			//odata = d->rl_command;
		}
		break;

	case 0x2:
		if (writeflag == MEM_WRITE) {
		/*	uint8_t old = d->rl_eecmd;
			d->rl_eecmd = idata;

			if (!d->eeprom_selected && d->rl_eecmd & RL_EE_SEL) {
				d->eeprom_cur_cmd = 0;
				d->eeprom_cur_cmd_bit = 0;
			}
			d->eeprom_selected = d->rl_eecmd & RL_EE_SEL;

			if (idata & RL_EE_CLK && !(old & RL_EE_CLK))
				eeprom_clk(d);*/
		} else {
			//odata = d->rl_eecmd;
		}
		break;

	default:
		if (writeflag == MEM_WRITE) {
			fatal("[ hardfoo: unimplemented write to "
			    "offset 0x%x: data=0x%x ]\n", (int)
			    relative_addr, (int)idata);
		} else {
			fatal("[ hardfoo: unimplemented read from "
			    "offset 0x%x ]\n", (int)relative_addr);
		}
		exit(1);
	}

	if (writeflag == MEM_READ)
		memory_writemax64(cpu, data, len, odata);

	return 1;
}


DEVINIT(hardfoo)
{
	char *name2;
	size_t nlen = 100;
	struct hardfoo_data *d;

	CHECK_ALLOCATION(d = (struct hardfoo_data *) malloc(sizeof(struct hardfoo_data)));
	memset(d, 0, sizeof(struct hardfoo_data));

	CHECK_ALLOCATION(name2 = (char *) malloc(nlen));
	snprintf(name2, nlen, "hardfoo");

	memory_device_register(devinit->machine->memory, name2,
	    devinit->addr, DEV_HARDFOO_LENGTH, dev_hardfoo_access, (void *)d,
	    DM_DEFAULT, NULL);


	return 1;
}

