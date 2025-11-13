#include "xhci.h"

void xhci_reset(XHCIControllerSession *session){
	USBCMD = USBCMD | USBCMD_MASK_HCRST;
	int i = 10;
	while(USBCMD_HCRST&&i){
		sleep(10);
		i--;
	}
}