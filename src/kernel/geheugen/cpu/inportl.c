#include "geheugen.h"

uint32_t inportl(uint16_t port){
    uint32_t returnVal;
    asm volatile ("inl %1, %0"
    : "=a"(returnVal)
    : "Nd"(port));
    return returnVal;
}