#include "xhci.h"

void xhci_reset(XHCIControllerSession *session){
	USBCMD = USBCMD | USBCMD_MASK_HCRST;
	int i = 1000;
	while(USBCMD_HCRST&&i>0){
		sleep(10);
		i--;
	}
	if (i == 0) {
        printk("WAARSCHUWING: xHCI Host Controller Reset (HCRST) timeout!\n");
    } else {
        printd("xHCI Host Controller Reset succesvol voltooid.\n");
    }
}