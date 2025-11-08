#include "xhci.h"

void xhci_stop()
{
	if(USBCMD_RS){
		// yes, we do run!
		// we probably have a bios or uefi who booted us
		USBCMD = 0;
		sleep(10);
		// now wait until we have come to a full stop
		while(USBSTS_HCH==0);
        printk("xhci_stop: controller stopped.\n");
	}else{
        printk("xhci_stop: controller already stopped.\n");
    }
}