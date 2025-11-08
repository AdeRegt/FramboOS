#include "xhci.h"

uint64_t device_context_base_address_array[XHCI_MAX_SLOTS + 1] __attribute__((aligned(64)));