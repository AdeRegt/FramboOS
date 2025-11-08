#include "xhci.h"

uint32_t xhci_event_ring[XHCI_EVENT_RING_SIZE*4] __attribute__((aligned(64)));