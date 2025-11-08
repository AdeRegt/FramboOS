#include "xhci.h"

void xhci_setup_eventring()
{
    //
    // Initialiseer de Event Ring Segment Table
    //
    memset((void*)&xhci_event_ring_segment_table,0,sizeof(XHCIEventRingSegmentTable));
    xhci_event_ring_segment_table.ring_segment_base_address_low = ((uint32_t)((uint64_t)xhci_event_ring));
	xhci_event_ring_segment_table.ring_segment_base_address_high = 0;
	xhci_event_ring_segment_table.ring_segment_size = XHCI_EVENT_RING_SIZE;

    //
    // Initialiseer de Event Ring
    //
    for (int i = 0; i < XHCI_EVENT_RING_SIZE*4; i++)
    {
        xhci_event_ring[i] = 0;
    }

    //
    // Stel de Event Ring Segment Table Pointer in
    //
    ERSTSZ(0) = 1; // Aantal segmenten - 1
    ERSTBA_L(0) = (uint64_t)(uintptr_t)&xhci_event_ring_segment_table;
    ERSTBA_H(0) = 0;

    //
    // Stel de Event Ring Dequeue Pointer in
    //
    ERDP_L(0) = (uint64_t)(uintptr_t)xhci_event_ring;
    ERDP_H(0) = 0;

    printk("xhci_setup_eventring: Event Ring ingesteld op %x met grootte %d \n", xhci_event_ring, XHCI_EVENT_RING_SIZE);
}