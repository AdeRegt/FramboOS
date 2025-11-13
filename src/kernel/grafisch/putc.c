#include "beeldscherm.h"

void putc(char t)
{
    if(t=='\n')
    {
        beeldscherm_x = BEELDSCHERM_OFFSET_X;
        beeldscherm_y += BEELDSCHERM_FONT_HEIGHT + BEELDSCHERM_FONT_RUIMTE;
        if(beeldscherm_y > graphics_info->Height){
            beeldscherm_leeg();
            beeldscherm_x = BEELDSCHERM_OFFSET_X;
            beeldscherm_y = BEELDSCHERM_OFFSET_Y;
        }
        return;
    }
    if(t=='\t')
    {
        beeldscherm_x = ((beeldscherm_x - BEELDSCHERM_OFFSET_X + (4 * (BEELDSCHERM_FONT_WIDTH + BEELDSCHERM_FONT_RUIMTE))) / (4 * (BEELDSCHERM_FONT_WIDTH + BEELDSCHERM_FONT_RUIMTE))) * (4 * (BEELDSCHERM_FONT_WIDTH + BEELDSCHERM_FONT_RUIMTE)) + BEELDSCHERM_OFFSET_X;
        return;
    }
    drawCharacterInDefaultFont(t, BEELDSCHERM_FONT_COLOR1, beeldscherm_x+BEELDSCHERM_FONT_SCHADUW, beeldscherm_y+BEELDSCHERM_FONT_SCHADUW, 1);
    drawCharacterInDefaultFont(t, BEELDSCHERM_FONT_COLOR2, beeldscherm_x, beeldscherm_y, 0);
    beeldscherm_x += BEELDSCHERM_FONT_WIDTH  + BEELDSCHERM_FONT_RUIMTE;
}