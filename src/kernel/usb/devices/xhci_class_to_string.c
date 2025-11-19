#include "xhci.h"

char* xhci_class_to_string(uint8_t class_code)
{
    switch(class_code){
        case 0x00:
            return "Per Interface";
        case 0x01:
            return "Audio";
        case 0x02:
            return "Communications and CDC Control";
        case 0x03:
            return "HID (Human Interface Device)";
        case 0x05:
            return "Physical";
        case 0x06:
            return "Image";
        case 0x07:
            return "Printer";
        case 0x08:
            return "Mass Storage";
        case 0x09:
            return "Hub";
        case 0x0A:
            return "CDC-Data";
        case 0x0B:
            return "Smart Card";
        case 0x0D:
            return "Content Security";
        case 0x0E:
            return "Video";
        case 0x0F:
            return "Personal Healthcare";
        case 0x10:
            return "Audio/Video Devices";
        case 0x11:
            return "Billboard Device Class";
        case 0x12:
            return "USB Type-C Bridge Class";
        case 0xDC:
            return "Diagnostic Device";
        case 0xE0:
            return "Wireless Controller";
        case 0xEF:
            return "Miscellaneous";
        case 0xFE:
            return "Application Specific";
        case 0xFF:
            return "Vendor Specific";
        default:
            return "Unknown Class";
    }
}