#include "xhci.h"

XHCIControllerSession* xhci_allocate_new_session()
{
    XHCIControllerSession* session = &xhci_session[xhci_session_count++];
    return session;
}