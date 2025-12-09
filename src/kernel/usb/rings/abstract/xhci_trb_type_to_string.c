#include "xhci.h"

char* xhci_trb_type_to_string(uint8_t trb_type)
{
    switch(trb_type){
        // case XHCI_TRB_NORMAL_TRB_TYPE:
        //     return "Normal TRB";
        case XHCI_TRB_SETUP_TRB_TYPE:
            return "Setup Stage";
        case XHCI_TRB_DATA_TRB_TYPE:
            return "Data Stage";
        case XHCI_TRB_STATUS_TRB_TYPE:
            return "Status Stage";
        // case XHCI_TRB_ISOCH_COMMAND_TRB_TYPE:
        //     return "Isoch Command TRB";
        // case XHCI_TRB_LINK_TRB_TYPE:
        //     return "Link TRB";
        // case XHCI_TRB_EVENT_DATA_TRB_TYPE:
        //     return "Event Data TRB";
        // case XHCI_TRB_NO_OP_COMMAND_TRB_TYPE:
        //     return "No-Op Command TRB";
        case XHCI_TRB_ENABLE_SLOT_COMMAND_TRB_TYPE:
            return "Enable Slot Command";
        // case XHCI_TRB_DISABLE_SLOT_COMMAND_TRB_TYPE:
        //     return "Disable Slot Command TRB";
        case XHCI_TRB_SET_ADDRESS_COMMAND_TRB_TYPE:
            return "Address Device Command";
        case XHCI_TRB_CONFIGURE_ENDPOINT_COMMAND_TRB_TYPE:
            return "Configure Endpoint Command";
        // case XHCI_TRB_EVALUATE_CONTEXT_COMMAND_TRB_TYPE:
        //     return "Evaluate Context Command TRB";
        // case XHCI_TRB_RESET_ENDPOINT_COMMAND_TRB_TYPE:
        //     return "Reset Endpoint Command TRB";
        // case XHCI_TRB_STOP_ENDPOINT_COMMAND_TRB_TYPE:
        //     return "Stop Endpoint Command TRB";
        // case XHCI_TRB_SET_TRANSFER_RING_DEQUEUE_POINTER_COMMAND_TRB_TYPE:
        //     return "Set Transfer Ring Dequeue Pointer Command TRB";
        // case XHCI_TRB_RESET_DEVICE_COMMAND_TRB_TYPE:
        //     return "Reset Device Command TRB";
        // case XHCI_TRB_FORCE_EVENT_COMMAND_TRB_TYPE:
        //     return "Force Event Command TRB";
        // case XHCI_TRB_NEGOTIATE_BANDWIDTH_COMMAND_TRB_TYPE:
        //     return "Negotiate Bandwidth Command TRB";
        // case XHCI_TRB_SET_LATENCY_TOLERANCE_VALUE_COMMAND_TRB_TYPE:
        //     return "Set Latency Tolerance Value Command TRB";
        // case XHCI_TRB_GET_PORT_BANDWIDTH_COMMAND_TRB_TYPE:
        //     return "Get Port Bandwidth Command TRB";
        // case XHCI_TRB_FORCE_HEADER_COMMAND_TRB_TYPE:
        //     return "Force Header Command TRB";
        // case XHCI_TRB_NO_OP_EVENT_TRB_TYPE:
        //     return "No-Op Event TRB";
        // case XHCI_TRB_TRANSFER_EVENT_TRB_TYPE:
        //     return "Transfer Event TRB";
        // case XHCI_TRB_COMMAND_COMPLETION_EVENT_TRB_TYPE:
        //     return "Command Completion Event TRB";
        // case XHCI_TRB_PORT_STATUS_CHANGE_EVENT_TRB_TYPE:
        //     return "Port Status Change Event TRB";
        // case XHCI_TRB_BANDWIDTH_REQUEST_EVENT_TRB_TYPE:
        //     return "Bandwidth Request Event TRB";
        // case XHCI_TRB_DOORBELL_EVENT_TRB_TYPE:
        //     return "Doorbell Event TRB";
        // case XHCI_TRB_HOST_CONTROLLER_EVENT_TRB_TYPE:
        //     return "Host Controller Event TRB";
        // case XHCI_TRB_DEVICE_NOTIFICATION_EVENT_TRB_TYPE:
        //     return "Device Notification Event TRB";
        // case XHCI_TRB_MFINDEX_WRAP_EVENT_TRB_TYPE:
        //     return "MFIndex Wrap Event TRB";
        default:
            return "Onbekend Type";
    }
}