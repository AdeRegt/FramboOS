#include "serialport.h"

uint8_t ontvang_com()
{
    while((inportb(COMPORT+5)&1)==0);
    inportb(COMPORT);
}