#include "geheugen.h"
#include "debugger.h"

#define COMPORT 0x3f8

void laad_com();
uint8_t ontvang_com();
void verstuur_com(uint8_t token);