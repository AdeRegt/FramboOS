#include "beeldscherm.h"

void beeldscherm_laad(GraphicsInfo *grafische_info)
{

  //
  // de grafische info opslaan
  graphics_info = grafische_info;

  //
  // het scherm leegmaken
  beeldscherm_leeg();

  //
  // een font inladen...
  printk("Grafisch systeem geladen met resolutie: %dx%d\n", graphics_info->Width, graphics_info->Height);
}
