#include "xhci.h"

char* xhci_get_resultcode_string(uint8_t code)
{
    switch(code) {
        case 1:
            return "Success";
        case 2:
            return "Data Buffer Error";
        case 3:
            return "Babble Detected";
        case 4:
            return "USB Transaction Error";
        case 5:
            return "TRB Error";
        case 6:
            return "Stall Error";
        case 7:
            return "Resource Error";
        case 8:
            return "Bandwidth Error";
        case 9:
            return "No Slots Available";
        case 10:
            return "Invalid Stream Type";
        case 11:
            return "Slot Not Enabled";
        case 12:
            return "Endpoint Not Enabled";
        case 13:
            return "Short Packet";
        case 14:
            return "Ring Underrun";
        case 15:
            return "Ring Overrun";
        case 16:
            return "VF Event Ring Full";
        case 17:
            return "Parameter Error";
        default:
            return "Unknown Result Code";
    }
}