#include "xhci.h"

uint32_t xhci_command_ring[XHCI_COMMAND_RING_SIZE*4] __attribute__((aligned(64)));