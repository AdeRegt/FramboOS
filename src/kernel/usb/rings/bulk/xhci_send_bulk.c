#include "xhci.h"

void xhci_send_bulk(XHCIControllerSession *session, USBDevice* device, USBRing *ring, uint64_t data_length, void* data){
    TransferTRB* trb1 = (TransferTRB*) xhci_alloc_trb_ring(ring);
    trb1->DataBufferPointerHi = 0;
    trb1->DataBufferPointerLo = (uint32_t) (uint64_t) data;
    trb1->BlockEventInterrupt = 0;
    trb1->Chainbit = 0;
    trb1->Cyclebit = 1;
    trb1->EvaluateNextTRB = 0;
    trb1->ImmediateData = 0;
    trb1->InterrupterTarget = 0;
    trb1->InterruptOnCompletion = 1;
    trb1->InterruptonShortPacket = 0;
    trb1->NoSnoop = 0;
    trb1->TRBTransferLength = data_length;
    trb1->TRBType = 1;

    xhci_ring_trb_ring(session, device, ring, (void*)trb1);
}