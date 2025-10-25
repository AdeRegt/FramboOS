#include "beeldscherm.h"

void drawCharacter(PSF1_Font *font,char character,unsigned int colour,unsigned long xOff,unsigned long yOff){
    char* fontPtr = font->glyphBuffer + (character * font->psf1_Header->charsize);
    for (unsigned long y = yOff; y < yOff + font->psf1_Header->charsize; y++){
        for (unsigned long x = xOff; x < xOff+8; x++){
            if ((*fontPtr & (0b10000000 >> (x - xOff))) > 0){
                    beeldscherm_plaats_pixel(x,y,colour);
            }

        }
        fontPtr++;
    }
}