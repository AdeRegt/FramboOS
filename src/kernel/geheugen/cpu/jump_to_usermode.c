#include "geheugen.h"

uint64_t uprogstart;
uint64_t usermode_eflags;

void jump_to_usermode(uint64_t addr){
    uprogstart = addr;
    usermode_eflags = 0x2002;
    extern void jump_usermode();
    jump_usermode();
}