#include "xhci.h"

void xhci_setup_eventring(XHCIControllerSession *session)
{
    session->xhci_event_ring_segment_table = (XHCIEventRingSegmentTable*) alloc_page();
    session->xhci_event_ring = (uint32_t*) alloc_page();
    
    //
    // Initialiseer de Event Ring Segment Table
    //
    memset((void*)session->xhci_event_ring_segment_table,0,sizeof(XHCIEventRingSegmentTable));
    session->xhci_event_ring_segment_table->ring_segment_base_address_low = ((uint32_t)((uint64_t)session->xhci_event_ring));
	session->xhci_event_ring_segment_table->ring_segment_base_address_high = 0;
	session->xhci_event_ring_segment_table->ring_segment_size = XHCI_EVENT_RING_SIZE;

    //
    // Initialiseer de Event Ring
    //
    for (int i = 0; i < XHCI_EVENT_RING_SIZE*4; i++)
    {
        session->xhci_event_ring[i] = 0;
    }

    //
    // Stel de Event Ring Segment Table Pointer in
    //
    ERSTSZ(0) = 1; // Aantal segmenten - 1
    ERSTBA_L(0) = (uint64_t)(uintptr_t)session->xhci_event_ring_segment_table;
    ERSTBA_H(0) = 0;

    //
    // Stel de Event Ring Dequeue Pointer in
    //
    ERDP_L(0) = (uint64_t)(uintptr_t)session->xhci_event_ring;
    ERDP_H(0) = 0;

    printk("xhci_setup_eventring: Event Ring ingesteld op %x met grootte %d \n", session->xhci_event_ring, XHCI_EVENT_RING_SIZE);
}