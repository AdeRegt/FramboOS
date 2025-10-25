#include "beeldscherm.h"

void putc(char t)
{
    drawCharacterInDefaultFont(t, BEELDSCHERM_FONT_COLOR1, beeldscherm_x+BEELDSCHERM_FONT_SCHADUW, beeldscherm_y+BEELDSCHERM_FONT_SCHADUW);
    drawCharacterInDefaultFont(t, BEELDSCHERM_FONT_COLOR2, beeldscherm_x, beeldscherm_y);
    beeldscherm_x += BEELDSCHERM_FONT_WIDTH  + BEELDSCHERM_FONT_RUIMTE;
}