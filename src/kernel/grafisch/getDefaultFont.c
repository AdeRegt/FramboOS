#include "beeldscherm.h"

PSF1_Font *getDefaultFont()
{
  char *dfb = getDefaultPSFFontBuffer();
  default_font.psf1_Header = (PSF1_Header *)(dfb);
  default_font.glyphBuffer = (void *)(dfb + sizeof(PSF1_Header));
  return (PSF1_Font *)&default_font;
}
