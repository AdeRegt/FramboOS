#include "xhci.h"

void perform_bios_handoff()
{
	uint32_t capapointer = (((uint32_t *)(base_xhci_address + 0x10))[0] >> 16) << 2;
	if (capapointer)
	{
		void *cappointer = capapointer + base_xhci_address;
		int captimeout = 10000;
		int i = 10;
		while (i)
		{
			i--;
			volatile uint32_t reg = ((volatile uint32_t *)cappointer)[0];
			uint8_t capid = reg & 0xFF;
			uint8_t capoffset = (reg >> 8) & 0xFF;
			if (capid == 0)
			{
				break;
			}
			if (capid == 1)
			{
				printk("perform_bios_handoff: BIOS/UEFI handoff gevonden.\n");
			}
			if (capid == 1 && reg & 0x10000)
			{
				printk("perform_bios_handoff: BIOS/UEFI handoff uitvoeren.\n");
				((volatile uint32_t *)cappointer)[0] |= 0x1000000;
				captimeout--;
				if (captimeout == 0)
				{
					break;
				}
				continue;
			}
			if (capoffset == 0)
			{
				break;
			}
			cappointer += capoffset * sizeof(uint32_t);
		}
	}
}