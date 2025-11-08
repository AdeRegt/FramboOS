#include "geheugen.h"

void sleep(uint32_t timer){
    for(uint32_t i = 0; i < timer * 10000; i++){
        __asm__ __volatile__ ("nop");
    }
}