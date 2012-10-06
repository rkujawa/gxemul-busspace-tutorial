/*
 *  Copyright (C) 2012  Radoslaw Kujawa.  All rights reserved.
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
 *  Fake Cards Advanced Addition Accelerator
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

#define	DEV_FAA_LENGTH	0x10

#define FAA_STATUS_BUSY 0x1

#define FAA_CMD_ADD	0x1

struct faa_data {
	uint8_t			faa_status;
	uint8_t			faa_command;
	uint32_t		faa_data;
	uint32_t		faa_result;
};


DEVICE_ACCESS(faa)
{
	struct faa_data *d = (struct faa_data *) extra;
	uint64_t idata = 0, odata = 0;

	if (writeflag == MEM_WRITE)
		idata = memory_readmax64(cpu, data, len);

	switch (relative_addr) {

	/* status register */
	case 0x0:
		if (writeflag == MEM_WRITE) {
	                fatal("[ faa: STATUS register (0x0) WRITE value %lx ]\n", idata);
			/* do nothing */
		} else {
			odata = d->faa_status;	
	                fatal("[ faa: STATUS register (0x0) READ value %lx ]\n", odata);
		}
		break;
	/* command register */
	case 0x4:
		if (writeflag == MEM_WRITE) {
	                fatal("[ faa: COMMAND register (0x4) WRITE value %lx ]\n", idata);
			//if (idata & FAA_CMD_RESET) {
				/* clear the reset bit: */
			//	idata &= ~RL_CMD_RESET;
			//}

			if (idata & FAA_CMD_ADD) {
		/*		d->faa_status |= FAA_STATUS_BUSY;
				d->faa_result = d->faa_data1 + d->faa_data2;
				d->faa_status &= ~FAA_STATUS_BUSY;
		*/
			}

			//d->faa_command = idata;

		} else {
			odata = d->faa_command;
	                fatal("[ faa: COMMAND register (0x4) READ value %lx ]\n", odata);
		}
		break;
	/* data register */
	case 0x8:
		if (writeflag == MEM_WRITE) {
	                fatal("[ faa: DATA register (0x8) WRITE value %lx ]\n", idata);
			d->faa_data = idata;
		} else {
			odata = d->faa_data;
	                fatal("[ faa: DATA register (0x8) READ value %lx ]\n", odata);
		}
		break;
	/* result register */
	case 0xC:
		if (writeflag == MEM_WRITE) {
	                fatal("[ faa: RESULT register (0xC) WRITE value %lx ]\n", idata);
			/* do nothing */
		} else {
			odata = d->faa_result;
	                fatal("[ faa: RESULT register (0xC) READ value %lx ]\n", odata);
		}
		break;

	default:
		if (writeflag == MEM_WRITE) {
			fatal("[ faa: unimplemented write to "
			    "offset 0x%x: data=0x%x ]\n", (int)
			    relative_addr, (int)idata);
		} else {
			fatal("[ faa: unimplemented read from "
			    "offset 0x%x ]\n", (int)relative_addr);
		}
		exit(1);
	}

	if (writeflag == MEM_READ)
		memory_writemax64(cpu, data, len, odata);

	return 1;
}


DEVINIT(faa)
{
	char *name2;
	size_t nlen = 100;
	struct faa_data *d;

	CHECK_ALLOCATION(d = (struct faa_data *) malloc(sizeof(struct faa_data)));
	memset(d, 0, sizeof(struct faa_data));

	CHECK_ALLOCATION(name2 = (char *) malloc(nlen));
	snprintf(name2, nlen, "faa");

	memory_device_register(devinit->machine->memory, name2,
	    devinit->addr, DEV_FAA_LENGTH, dev_faa_access, (void *)d,
	    DM_DEFAULT, NULL);


	return 1;
}

