/*
	Copyright (C) 2012 Skrilax_CZ (skrilax@gmail.com)
	
	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.
	
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	
	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
	*/

/* This uttility sets boot partition for A500 Bootlodaer */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

struct msc_message
{
	/* Boot commands:
	 * FastbootMode - fastboot
	 * FOTA - recovery 
	 */
	char boot_command[0x0C];
	
	/* Debug mode:
	 * 00 - off
	 * 01 - on
	 */
	unsigned char debug_mode;
	
	/* Primary boot partition:
	 * 00 - LNX
	 * 01 - AKB
	 */
	unsigned char boot_mode;
};

#define MSC_PARTITION "/dev/block/mmcblk0p5"

void print_usage()
{
	fprintf(stderr, "Usage1: setbootmode [b1|b2]\n");
	fprintf(stderr, "b1 for booting primary kernel image\n");
	fprintf(stderr, "b2 for booting secondary kernel image\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "Usage2: setbootmode --status\n");
    fprintf(stderr, "shows the current bootmode and exits\n");
}

int main(int argc, char** argv)
{
	struct msc_message msg;
	unsigned char boot_mode;
    unsigned char check_only=0;
	FILE* f;
	
	if (argc < 2)
	{
		print_usage();
		return 1;
	}
    else if (!strcmp(argv[1], "--status"))
        check_only=1;
	else if (!strcmp(argv[1], "b1"))
		boot_mode = 0;
	else if (!strcmp(argv[1], "b2"))
		boot_mode = 1;
	else
	{
		print_usage();
		return 1;
	}
	
    if(!check_only) /* read-write */
	    f = fopen(MSC_PARTITION, "rb+");
    else            /* read-only */
        f = fopen(MSC_PARTITION, "rb");
	if (f == NULL)
	{
		fprintf(stderr, "MSC partition not found.\n");
		return 1;
	}
	
	/* Read current msc message */
	fseek(f, 0x00, SEEK_SET);
	fread(&msg, 1, sizeof(struct msc_message), f);
	
    if(! check_only)
    {
	    /* Set boot mode */
	    msg.boot_mode = boot_mode;
	
	    /* Write it back */
	    fseek(f, 0x00, SEEK_SET);
	    fwrite(&msg, 1, sizeof(struct msc_message), f);
	}
	/* Done */
	fclose(f);
	
    if(check_only)
        printf("Currently booting %s kernel image\n",
               msg.boot_mode ? "secondary" : "primary");
	else if (boot_mode == 0)
		printf("Set to boot primary kernel image.\n");
	else if (boot_mode == 1)
		printf("Set to boot secondary kernel image.\n");
	
	return 0;
}
