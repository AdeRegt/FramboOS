#include "pci.h"

#define CAPLENGTH ((uint8_t*)session->base_xhci_address)[0]
#define HCIVERSION ((uint16_t*)( session->base_xhci_address + 0x02 ))[0]
#define HCSPARAMS1 ((uint32_t*)( session->base_xhci_address + 0x04 ))[0]
#define HCSPARAMS2 ((uint32_t*)( session->base_xhci_address + 0x08 ))[0]
#define HCSPARAMS3 ((uint32_t*)( session->base_xhci_address + 0x0C ))[0]
#define HCCPARAMS1 ((uint32_t*)( session->base_xhci_address + 0x10 ))[0]
#define DBOFF ((uint32_t*)( session->base_xhci_address + 0x14 ))[0]
#define RTSOFF ((uint32_t*)( session->base_xhci_address + 0x18 ))[0]
#define HCCPARAMS2 ((uint32_t*)( session->base_xhci_address + 0x1C ))[0]
#define VTIOSOFF ((uint32_t*)( session->base_xhci_address + 0x20 ))[0]

#define USBCMD ((uint32_t*)( session->base_xhci_address + CAPLENGTH + 0x00 ))[0]
#define USBSTS ((uint32_t*)( session->base_xhci_address + CAPLENGTH + 0x04 ))[0]
#define PAGESIZE ((uint32_t*)( session->base_xhci_address + CAPLENGTH + 0x08 ))[0]
#define DNCTRL ((uint32_t*)( session->base_xhci_address + CAPLENGTH + 0x14 ))[0]
#define CRCR_L ((uint32_t*)( session->base_xhci_address + CAPLENGTH + 0x18 ))[0]
#define CRCR_H ((uint32_t*)( session->base_xhci_address + CAPLENGTH + 0x18 + 4 ))[0]
#define DCBAAP_L ((uint32_t*)( session->base_xhci_address + CAPLENGTH + 0x30 ))[0]
#define DCBAAP_H ((uint32_t*)( session->base_xhci_address + CAPLENGTH + 0x30 + 4 ))[0]
#define CONFIG ((uint32_t*)( session->base_xhci_address + CAPLENGTH + 0x38 ))[0]
#define PORTSC(n) ((uint32_t*)( session->base_xhci_address + CAPLENGTH + (0x400 + (0x10 * n)) ))[0]

#define MFINDEX ((uint32_t*)( session->base_xhci_address + RTSOFF + 0x00 ))[0]
#define IMAN(n) ((uint32_t*)( session->base_xhci_address + RTSOFF + 0x20 + (32*n) ))[0]
#define IMOD(n) ((uint32_t*)( session->base_xhci_address + RTSOFF + 0x24 + (32*n) ))[0]
#define ERSTSZ(n) ((uint32_t*)( session->base_xhci_address + RTSOFF + 0x28 + (32*n) ))[0]
#define ERSTBA_L(n) ((uint64_t*)( session->base_xhci_address + RTSOFF + 0x30 + (32*n) ))[0]
#define ERSTBA_H(n) ((uint64_t*)( session->base_xhci_address + RTSOFF + 0x30 + 4 + (32*n) ))[0]
#define ERDP_L(n) ((uint64_t*)( session->base_xhci_address + RTSOFF + 0x38 + (32*n) ))[0]
#define ERDP_H(n) ((uint64_t*)( session->base_xhci_address + RTSOFF + 0x38 + 4 + (32*n) ))[0]

#define DOORBELL ((uint32_t*) ( session->base_xhci_address + DBOFF ))

#define HCSPARAMS1_MASK_MaxSlots 0xFF
#define HCSPARAMS1_MaxSlots ( HCSPARAMS1 & HCSPARAMS1_MASK_MaxSlots )

#define HCSPARAMS1_MASK_MaxIntrs 0x7FF00
#define HCSPARAMS1_SHIFT_MaxIntrs 8
#define HCSPARAMS1_MaxIntrs ((HCSPARAMS1 & HCSPARAMS1_MASK_MaxIntrs)>>HCSPARAMS1_SHIFT_MaxIntrs)

#define HCSPARAMS1_MASK_MaxPorts 0xFF000000
#define HCSPARAMS1_SHIFT_MaxPorts 24
#define HCSPARAMS1_MaxPorts ((HCSPARAMS1 & HCSPARAMS1_MASK_MaxPorts)>>HCSPARAMS1_SHIFT_MaxPorts)

#define HCSPARAMS2_MASK_IST 0xF
#define HCSPARAMS2_IST ( HCSPARAMS2 & HCSPARAMS2_MASK_IST )

#define HCSPARAMS2_MASK_ERST_Max 0xF0
#define HCSPARAMS2_SHIFT_ERST_Max 4
#define HCSPARAMS2_ERST_Max ( ( HCSPARAMS2 & HCSPARAMS2_MASK_ERST_Max ) >> HCSPARAMS2_SHIFT_ERST_Max )

#define HCSPARAMS2_MASK_Scratchpad_Bufs_Hi 0x3E00000
#define HCSPARAMS2_SHIFT_Scratchpad_Bufs_Hi 21
#define HCSPARAMS2_Scratchpad_Bufs_Hi ( ( HCSPARAMS2 & HCSPARAMS2_MASK_Scratchpad_Bufs_Hi ) >> HCSPARAMS2_SHIFT_Scratchpad_Bufs_Hi )

#define HCSPARAMS2_MASK_SPR 0x4000000
#define HCSPARAMS2_SHIFT_SPR 26
#define HCSPARAMS2_SPR ( ( HCSPARAMS2 & HCSPARAMS2_MASK_SPR ) >> HCSPARAMS2_SHIFT_SPR )

#define HCSPARAMS2_MASK_Scratchpad_Bufs_Lo 0x4000000
#define HCSPARAMS2_SHIFT_Scratchpad_Bufs_Lo 27
#define HCSPARAMS2_Scratchpad_Bufs_Lo ( ( HCSPARAMS2 & HCSPARAMS2_MASK_Scratchpad_Bufs_Lo ) >> HCSPARAMS2_SHIFT_Scratchpad_Bufs_Lo )

#define HCSPARAMS3_MASK_U1 0xFF
#define HCSPARAMS3_SHIFT_U1 0
#define HCSPARAMS3_U1 ( ( HCSPARAMS3 & HCSPARAMS3_MASK_U1 ) >> HCSPARAMS3_SHIFT_U1 )

#define HCSPARAMS3_MASK_U2 0xFFFF0000
#define HCSPARAMS3_SHIFT_U2 16
#define HCSPARAMS3_U2 ( ( HCSPARAMS3 & HCSPARAMS3_MASK_U2 ) >> HCSPARAMS3_SHIFT_U2 )

#define HCCPARAMS1_AC64       ( ( HCCPARAMS1 & 0b00000000000000000000000000000001 ) >> 0 )
#define HCCPARAMS1_BNC        ( ( HCCPARAMS1 & 0b00000000000000000000000000000010 ) >> 1 )
#define HCCPARAMS1_CSZ        ( ( HCCPARAMS1 & 0b00000000000000000000000000000100 ) >> 2 )
#define HCCPARAMS1_PPC        ( ( HCCPARAMS1 & 0b00000000000000000000000000001000 ) >> 3 )
#define HCCPARAMS1_PIND       ( ( HCCPARAMS1 & 0b00000000000000000000000000010000 ) >> 4 )
#define HCCPARAMS1_LHRC       ( ( HCCPARAMS1 & 0b00000000000000000000000000100000 ) >> 5 )
#define HCCPARAMS1_LTC        ( ( HCCPARAMS1 & 0b00000000000000000000000001000000 ) >> 6 )
#define HCCPARAMS1_NSS        ( ( HCCPARAMS1 & 0b00000000000000000000000010000000 ) >> 7 )
#define HCCPARAMS1_PAE        ( ( HCCPARAMS1 & 0b00000000000000000000000100000000 ) >> 8 )
#define HCCPARAMS1_SPC        ( ( HCCPARAMS1 & 0b00000000000000000000001000000000 ) >> 9 )
#define HCCPARAMS1_SEC        ( ( HCCPARAMS1 & 0b00000000000000000000010000000000 ) >> 10 )
#define HCCPARAMS1_CFC        ( ( HCCPARAMS1 & 0b00000000000000000000100000000000 ) >> 11 )
#define HCCPARAMS1_MaxPSASize ( ( HCCPARAMS1 & 0b00000000000000001111000000000000 ) >> 12 )
#define HCCPARAMS1_xECP       ( ( HCCPARAMS1 & 0b11111111111111110000000000000000 ) >> 16 )

#define USBCMD_MASK_RS        0b00000000000000000000000000000001
#define USBCMD_SHIFT_RS        0b00000000000000000000000000000000
#define USBCMD_RS             ( ( USBCMD & USBCMD_MASK_RS ) >> USBCMD_SHIFT_RS )
#define USBCMD_MASK_HCRST     0b00000000000000000000000000000010
#define USBCMD_SHIFT_HCRST     1
#define USBCMD_HCRST          ( ( USBCMD & USBCMD_MASK_HCRST ) >> USBCMD_SHIFT_HCRST )
#define USBCMD_MASK_INTE        0b00000000000000000000000000000100
#define USBCMD_INTE           ( ( USBCMD & 0b00000000000000000000000000000100 ) >> 2 )
#define USBCMD_HSEE           ( ( USBCMD & 0b00000000000000000000000000001000 ) >> 3 )
#define USBCMD_LHCRST         ( ( USBCMD & 0b00000000000000000000000010000000 ) >> 7 )
#define USBCMD_CSS            ( ( USBCMD & 0b00000000000000000000000100000000 ) >> 8 )
#define USBCMD_CRS            ( ( USBCMD & 0b00000000000000000000001000000000 ) >> 9 )
#define USBCMD_EWE            ( ( USBCMD & 0b00000000000000000000010000000000 ) >> 10 )
#define USBCMD_EU3S           ( ( USBCMD & 0b00000000000000000000100000000000 ) >> 11 )
#define USBCMD_CME            ( ( USBCMD & 0b00000000000000000010000000000000 ) >> 13 )
#define USBCMD_ETE            ( ( USBCMD & 0b00000000000000000100000000000000 ) >> 14 )
#define USBCMD_TSC_EN         ( ( USBCMD & 0b00000000000000001000000000000000 ) >> 15 )
#define USBCMD_VTIOE          ( ( USBCMD & 0b00000000000000010000000000000000 ) >> 16 )

#define USBSTS_HCH            ( ( USBSTS & 0b00000000000000000000000000000001 ) >> 0 )
#define USBSTS_HSE            ( ( USBSTS & 0b00000000000000000000000000000100 ) >> 2 )
#define USBSTS_EINT           ( ( USBSTS & 0b00000000000000000000000000001000 ) >> 3 )
#define USBSTS_PCD            ( ( USBSTS & 0b00000000000000000000000000010000 ) >> 4 )
#define USBSTS_SSS            ( ( USBSTS & 0b00000000000000000000000100000000 ) >> 8 )
#define USBSTS_RSS            ( ( USBSTS & 0b00000000000000000000001000000000 ) >> 9 )
#define USBSTS_SRE            ( ( USBSTS & 0b00000000000000000000010000000000 ) >> 10 )
#define USBSTS_CNR            ( ( USBSTS & 0b00000000000000000000100000000000 ) >> 11 )
#define USBSTS_HCE            ( ( USBSTS & 0b00000000000000000001000000000000 ) >> 12 )

#define PORTSC_CCS(n)            ( ( PORTSC(n) & 0b00000000000000000000000000000001 ) >> 0 )
#define PORTSC_PED(n)            ( ( PORTSC(n) & 0b00000000000000000000000000000010 ) >> 1 )
#define PORTSC_OCA(n)            ( ( PORTSC(n) & 0b00000000000000000000000000001000 ) >> 3 )
#define PORTSC_PR(n)             ( ( PORTSC(n) & 0b00000000000000000000000000010000 ) >> 4 )
#define PORTSC_PLS(n)            ( ( PORTSC(n) & 0b00000000000000000000000111100000 ) >> 5 )
#define PORTSC_PP(n)             ( ( PORTSC(n) & 0b00000000000000000000001000000000 ) >> 9 )
#define PORTSC_Port_Speed(n)     ( ( PORTSC(n) & 0b00000000000000000011110000000000 ) >> 10 )
#define PORTSC_PIC(n)            ( ( PORTSC(n) & 0b00000000000000001100000000000000 ) >> 14 )
#define PORTSC_LWS(n)            ( ( PORTSC(n) & 0b00000000000000010000000000000000 ) >> 16 )
#define PORTSC_CSC(n)            ( ( PORTSC(n) & 0b00000000000000100000000000000000 ) >> 17 )
#define PORTSC_PEC(n)            ( ( PORTSC(n) & 0b00000000000001000000000000000000 ) >> 18 )
#define PORTSC_WRC(n)            ( ( PORTSC(n) & 0b00000000000010000000000000000000 ) >> 19 )
#define PORTSC_OCC(n)            ( ( PORTSC(n) & 0b00000000000100000000000000000000 ) >> 20 )
#define PORTSC_PRC(n)            ( ( PORTSC(n) & 0b00000000001000000000000000000000 ) >> 21 )
#define PORTSC_PLC(n)            ( ( PORTSC(n) & 0b00000000010000000000000000000000 ) >> 22 )
#define PORTSC_CEC(n)            ( ( PORTSC(n) & 0b00000000100000000000000000000000 ) >> 23 )
#define PORTSC_CAS(n)            ( ( PORTSC(n) & 0b00000001000000000000000000000000 ) >> 24 )
#define PORTSC_WCE(n)            ( ( PORTSC(n) & 0b00000010000000000000000000000000 ) >> 25 )
#define PORTSC_WDE(n)            ( ( PORTSC(n) & 0b00000100000000000000000000000000 ) >> 26 )
#define PORTSC_WOE(n)            ( ( PORTSC(n) & 0b00001000000000000000000000000000 ) >> 27 )
#define PORTSC_DR(n)             ( ( PORTSC(n) & 0b01000000000000000000000000000000 ) >> 30 )
#define PORTSC_WPR(n)            ( ( PORTSC(n) & 0b10000000000000000000000000000000 ) >> 31 )

#define XHCI_EVENT_RING_SIZE 32
#define XHCI_SPEED_FULL   1
#define XHCI_SPEED_LOW    2
#define XHCI_SPEED_HI     3
#define XHCI_SPEED_SUPER  4

#define XHCI_PORT_LINK_U0 0
#define XHCI_PORT_LINK_U1 1
#define XHCI_PORT_LINK_U2 2
#define XHCI_PORT_LINK_U3 3
#define XHCI_PORT_LINK_DISABLED 4
#define XHCI_PORT_LINK_RXDETECT 5
#define XHCI_PORT_LINK_INACTIVE_STATE 6
#define XHCI_PORT_LINK_POLLING_STATE 7
#define XHCI_PORT_LINK_RECOVERY_STATE 8
#define XHCI_PORT_LINK_HOT_RESET_STATE 9
#define XHCI_PORT_LINK_COMPLIANCE_MODE_STATE 10
#define XHCI_PORT_LINK_TEST_STATE 11
#define XHCI_PORT_LINK_RESUME_STATE 15

#define XHCI_COMMAND_RING_SIZE 256

#define XHCI_ENDPOINT_TYPE_CONTROL 0
#define XHCI_ENDPOINT_TYPE_ISOCHRONOUS_OUT 1
#define XHCI_ENDPOINT_TYPE_BULK_OUT 2
#define XHCI_ENDPOINT_TYPE_INTERRUPT_OUT 3
#define XHCI_ENDPOINT_TYPE_ENDPOINTCONTEXT 4
#define XHCI_ENDPOINT_TYPE_ISOCHRONOUS_IN 5
#define XHCI_ENDPOINT_TYPE_BULK_IN 6
#define XHCI_ENDPOINT_TYPE_INTERRUPT_IN 7

#define XHCI_MAX_SLOTS 255

#define XHCI_CRCS_DEFAULT_CYCLE_STATE 1

typedef struct{
    uint32_t ring_segment_base_address_low;
    uint32_t ring_segment_base_address_high;
    uint16_t ring_segment_size;
    uint16_t reservedA;
    uint32_t reservedB;
}__attribute__((packed)) XHCIEventRingSegmentTable;

typedef struct {
    pci_class* pci_device;
    void* base_xhci_address;
    uint64_t* device_context_base_address_array;
    uint32_t* xhci_command_ring;
    XHCIEventRingSegmentTable* xhci_event_ring_segment_table;
    uint32_t* xhci_event_ring;
}__attribute__((packed)) XHCIControllerSession;

typedef struct {
    uint32_t reserved1:24;
    uint16_t PortID:8;
    uint32_t reserved2;
    uint32_t reserved3:24;
    uint16_t CompletionCode:8;
    uint8_t CycleBit:1;
    uint16_t reserved4:9;
    uint8_t TRBType:6;
    uint32_t reserved5:16;
}__attribute__((packed)) PortStatusChangeEventTransferRequestBlock;

extern XHCIControllerSession xhci_session[10];
extern int xhci_session_count;

// extern uint64_t device_context_base_address_array[XHCI_MAX_SLOTS + 1];
// extern uint32_t xhci_command_ring[XHCI_COMMAND_RING_SIZE*4];
// extern XHCIEventRingSegmentTable xhci_event_ring_segment_table;
// extern uint32_t xhci_event_ring[XHCI_EVENT_RING_SIZE*4];

void laad_xhci(pci_class* xhci_device);
__attribute__((interrupt)) void xhci_interrupt_handler(interrupt_frame* frame);
void perform_bios_handoff(XHCIControllerSession *session);
void xhci_stop(XHCIControllerSession *session);
void xhci_reset(XHCIControllerSession *session);
void wait_for_controller_not_ready(XHCIControllerSession *session);
void xhci_setup_dcbaap(XHCIControllerSession *session);
void xhci_setup_commandring(XHCIControllerSession *session);
void xhci_setup_eventring(XHCIControllerSession *session);
void xhci_set_max_ports(XHCIControllerSession *session);
void event_watcher();
void xhci_handle_port_change_event(XHCIControllerSession *session, PortStatusChangeEventTransferRequestBlock* psc_event);