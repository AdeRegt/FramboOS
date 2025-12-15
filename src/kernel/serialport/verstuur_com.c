#include "serialport.h"

void verstuur_com(uint8_t token)
{
    while((inportb(COMPORT+5)&0x20)==0);
    outportb(COMPORT,token);
}