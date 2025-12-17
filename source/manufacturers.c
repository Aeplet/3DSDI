#include "manufacturers.h"
#include "globals.h"

static void set_buffer_string(char* buffer, char* string) {
    snprintf(buffer, MANUFACTURER_BUFFER_SIZE, "%s", string);
}

// thanks hekate for the (mostly) complete list
void get_manufacturer_name(uint8_t mid, char* buffer) {
    switch (mid) {
        case 0x00:
            set_buffer_string(buffer, "FAKE");
            break;
        case 0x01:
            set_buffer_string(buffer, "Panasonic");
            break;
        case 0x02:
            set_buffer_string(buffer, "Toshiba");
            break;
        case 0x03:
            set_buffer_string(buffer, "SanDisk");
            break;
        case 0x06:
            set_buffer_string(buffer, "Ritek");
            break;
        case 0x09:
            set_buffer_string(buffer, "ATP");
            break;
        case 0x13:
            set_buffer_string(buffer, "Kingmax");
            break;
        case 0x19:
            set_buffer_string(buffer, "Dynacard");
            break;
        case 0x1A:
            set_buffer_string(buffer, "Power Quotient");
            break;
        case 0x1B:
            set_buffer_string(buffer, "Samsung");
            break;
        case 0x1D:
            set_buffer_string(buffer, "ADATA");
            break;
        case 0x27:
            set_buffer_string(buffer, "Phison");
            break;
        case 0x28:
            set_buffer_string(buffer, "Barun Electronics");
            break;
        case 0x31:
            set_buffer_string(buffer, "Silicon Power");
            break;
        case 0x41:
            set_buffer_string(buffer, "Kingston");
            break;
        case 0x51:
            set_buffer_string(buffer, "STEC");
            break;
        case 0x5D:
            set_buffer_string(buffer, "SwissBit");
            break;
        case 0x61:
            set_buffer_string(buffer, "Netlist");
            break;
        case 0x63:
            set_buffer_string(buffer, "Cactus");
            break;
        case 0x73:
            set_buffer_string(buffer, "Bongiovi");
            break;
        case 0x74:
            set_buffer_string(buffer, "Jiaelec");
            break;
        case 0x76:
            set_buffer_string(buffer, "Patriot");
            break;
        case 0x82:
            set_buffer_string(buffer, "Jiang Tay");
            break;
        case 0x83:
            set_buffer_string(buffer, "Netcom");
            break;
        case 0x84:
            set_buffer_string(buffer, "Strontium");
            break;
        case 0x9C:
            set_buffer_string(buffer, "Sony");
            break;
        case 0x9F:
            set_buffer_string(buffer, "Taishin");
            break;
        case 0xAD:
            set_buffer_string(buffer, "Longsys");
            break;
        default:
            set_buffer_string(buffer, "Unknown");
            break;
    }
}