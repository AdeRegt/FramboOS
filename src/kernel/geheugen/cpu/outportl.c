#include "geheugen.h"

void outportl(uint16_t port, uint32_t value){
    asm volatile ("outl %0, %1" : : "a"(value), "Nd"(port));
}