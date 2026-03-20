#include "xhci.h"

void xhci_device_event_router(XHCIControllerSession *session, USBDevice* device)
{
    create_inquery_command(session,device);
}