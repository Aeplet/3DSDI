#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <3ds.h>
#include <inttypes.h>

#include "globals.h"
#include "manufacturers.h"

bool failed = false;

void print_top() {
    printf("3DSDI %s\n", APP_VERSION);
    printf("Created by Aep\n\n");
    printf("Press START to exit\n");
}

#include <stdint.h>

// template from online
struct sd_cid {
    uint8_t  mid;          /* Manufacturer ID */
    char     oid[3];       /* OEM/Application ID */
    char     pnm[6];       /* Product name */
    uint8_t  prv_major;    /* Product revision major */
    uint8_t  prv_minor;    /* Product revision minor */
    uint32_t psn;          /* Product serial number */
    uint16_t year;         /* Manufacturing year */
    uint8_t  month;        /* Manufacturing month */
};

static uint8_t hex2(const char *s) {
    return (uint8_t)(
        ((s[0] <= '9') ? s[0] - '0' : (s[0] & 0xDF) - 'A' + 10) << 4 |
        ((s[1] <= '9') ? s[1] - '0' : (s[1] & 0xDF) - 'A' + 10)
    );
}

int parse_sd_cid(const char *cid, struct sd_cid *out) {
    uint8_t b[16];

    if (!cid || !out)
        return -1;

    for (int i = 0; i < 16; i++)
        b[i] = hex2(cid + i * 2);

    out->mid = b[0];

    out->oid[0] = b[1];
    out->oid[1] = b[2];
    out->oid[2] = 0;

    out->pnm[0] = b[3];
    out->pnm[1] = b[4];
    out->pnm[2] = b[5];
    out->pnm[3] = b[6];
    out->pnm[4] = b[7];
    out->pnm[5] = 0;

    out->prv_major = b[8] >> 4;
    out->prv_minor = b[8] & 0x0F;

    out->psn = (uint32_t)b[9]  << 24 |
               (uint32_t)b[10] << 16 |
               (uint32_t)b[11] << 8  |
               (uint32_t)b[12];

    uint16_t mdt = ((b[13] & 0x0F) << 8) | b[14];
    out->year  = 2000 + (mdt >> 4);
    out->month = mdt & 0x0F;

    return 0;
}

// credits to 3DSIdent, much appreciated (code below is under Zlib license, license text is below)
/*
Copyright (c) 2016 Wolfvak

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgement in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
 */

char* get_sd_cid(void) {
    static char cid[33];
    u8 buffer[16];

    Result ret = FSUSER_GetSdmcCid(buffer, sizeof(buffer));
    if (R_FAILED(ret)) {
        printf("Failed to get SDMC CID\n");
        return "<unknown>"; // ??
    }

    // reverse16 + rotate-left-by-1 in one go:
    for (int i = 0; i < 15; i++) {
        // final[i] = reversed[i+1]
        snprintf(cid + i*2, 3, "%02X", buffer[14 - i]);
    }
    // final[15] = reversed[0]
    snprintf(cid + 30, 3, "%02X", buffer[15]);

    cid[32] = '\0';
    return cid;
}

int main(int argc, char* argv[])
{
	gfxInitDefault();
	consoleInit(GFX_TOP, NULL);

	print_top();

    struct sd_cid cid_info;
    char* cid = get_sd_cid();
    printf("CID: %s\n\n", cid);

    if (parse_sd_cid(cid, &cid_info) == 0) {
        char manufacturer_name[MANUFACTURER_BUFFER_SIZE];
        get_manufacturer_name(cid_info.mid, manufacturer_name);
        printf("Manufacturer Name: %s\n", manufacturer_name);
        printf("Manufacturer ID: 0x%02X\n", cid_info.mid);
        printf("OEM ID: %s\n", cid_info.oid);
        printf("Product Name: %s\n", cid_info.pnm);
        printf("Product Revision: %u.%u\n", cid_info.prv_major, cid_info.prv_minor);
        printf("Product Serial Number: %" PRIu32 "\n", cid_info.psn);
        printf("Manufacturing Date: %u-%02u\n", cid_info.year, cid_info.month);
    }
    else {
        printf("Failed to parse SD CID!\n"); // how I do not know
        failed = true;
    }

	// Main loop
	while (aptMainLoop())
	{
		gspWaitForVBlank();
		gfxSwapBuffers();
		hidScanInput();

		// Your code goes here
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			break; // break in order to return to hbmenu

        if (failed) {
            printf("Exiting in 3 seconds...");
            usleep(3000000);
            break;
        }
	}

	gfxExit();
	return 0;
}