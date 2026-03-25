#pragma once
#include <stdint.h>

extern void* fshook;

void wachten_op_bestandssysteem();
void bs_regristreer(void* fsdata);
void bs_leeg();
void* bestand(uint8_t* path);
uint8_t* directory();