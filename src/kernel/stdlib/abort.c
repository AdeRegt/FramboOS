#include "lualib.h"

void abort(void) {
    // 1. Optioneel: Print een foutmelding (indien je printf/serial hebt)
    // printf("Kernel panic: abort() called\n");

    // 2. Stop de huidige thread of het hele systeem
    while (1) {
        __asm__ volatile("cli; hlt"); // Zet de CPU in een energiezuinige slaapstand
    }
}