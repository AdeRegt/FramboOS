#include "beeldscherm.h"
#include "serialport.h"

void laad_beeldscherm(GraphicsInfo *grafische_info)
{

  //
  // de grafische info opslaan
  graphics_info = grafische_info;

  //
  // het scherm leegmaken
  beeldscherm_leeg();

  #ifdef ENABLE_COM
  //
  // com laden
  laad_com();
  #endif

  //
  // een font inladen...
  printk("FramboOS build %s - %s\n", __DATE__, __TIME__);
  printk("Grafisch systeem geladen met resolutie: %dx%d\n", graphics_info->Width, graphics_info->Height);
}
