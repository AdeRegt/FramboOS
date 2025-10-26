#include "beeldscherm.h"

void drawCharacter(PSF1_Font *font, char character, unsigned int colour, unsigned long xOff, unsigned long yOff, uint8_t shadow)
{
    char *fontPtr = font->glyphBuffer + (character * font->psf1_Header->charsize);
    for (unsigned long y = yOff; y < yOff + font->psf1_Header->charsize; y++)
    {
        for (unsigned long x = xOff; x < xOff + 8; x++)
        {
            if ((*fontPtr & (0b10000000 >> (x - xOff))) > 0)
            {
                beeldscherm_plaats_pixel(x, y, colour);
                if(shadow)
                {
                    beeldscherm_plaats_pixel(x-1, y-1, BEELDSCHERM_FONT_COLOR1);
                    beeldscherm_plaats_pixel(x-1, y  , BEELDSCHERM_FONT_COLOR1);
                    beeldscherm_plaats_pixel(x  , y-1, BEELDSCHERM_FONT_COLOR1);
                    beeldscherm_plaats_pixel(x+1, y-1, BEELDSCHERM_FONT_COLOR1);
                    beeldscherm_plaats_pixel(x-1, y+1, BEELDSCHERM_FONT_COLOR1);
                    beeldscherm_plaats_pixel(x+1, y  , BEELDSCHERM_FONT_COLOR1);
                    beeldscherm_plaats_pixel(x  , y+1, BEELDSCHERM_FONT_COLOR1);
                    beeldscherm_plaats_pixel(x+1, y+1, BEELDSCHERM_FONT_COLOR1);
                }
            }
        }
        fontPtr++;
    }
}