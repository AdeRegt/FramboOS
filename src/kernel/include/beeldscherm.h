#include "bootloader.h"
#include "debugger.h"

typedef __builtin_va_list va_list;

#define va_start(a,b)  __builtin_va_start(a,b)
#define va_end(a)      __builtin_va_end(a)
#define va_arg(a,b)    __builtin_va_arg(a,b)
#define __va_copy(d,s) __builtin_va_copy((d),(s))

#define BEELDSCHERM_OFFSET_X 5 
#define BEELDSCHERM_OFFSET_Y 5

#define BEELDSCHERM_ACHTERGROND_KLEUR 0xFF0000

#define BEELDSCHERM_FONT_WIDTH 8
#define BEELDSCHERM_FONT_HEIGHT 16
#define BEELDSCHERM_FONT_COLOR1 0xFFFFFFFF
#define BEELDSCHERM_FONT_COLOR2 0x00000000
#define BEELDSCHERM_FONT_SCHADUW 1
#define BEELDSCHERM_FONT_RUIMTE 5

typedef struct
{
    uint8_t magic[2];
    uint8_t mode;
    uint8_t charsize;
} PSF1_Header;

typedef struct
{
    PSF1_Header *psf1_Header;
    void *glyphBuffer;
} PSF1_Font;

extern GraphicsInfo *graphics_info;
extern char default_psf[0x14c0];
extern PSF1_Font default_font;
extern int beeldscherm_x;
extern int beeldscherm_y;


/**
 * Laadt de grafische informatie en maakt het scherm leeg.
 */
void laad_beeldscherm(GraphicsInfo *grafische_info);

/**
 * Maakt het beeldscherm leeg door alle pixels op een vaste kleur te zetten.
 */
void beeldscherm_leeg();

/**
 * Plaatst een pixel op de opgegeven (x, y) coördinaten met de gegeven kleur.
 */
void beeldscherm_plaats_pixel(unsigned int x, unsigned int y, unsigned int colour);

void putc(char t);
char *convertl(uint64_t num, int base);
char *convert(uint32_t num, int base);
void print_raw_string(char *message);
void drawCharacter(PSF1_Font *font,char character,unsigned int colour,unsigned long xOff,unsigned long yOff, uint8_t shadow);
PSF1_Font *getDefaultFont();
char *getDefaultPSFFontBuffer();
void drawCharacterInDefaultFont(char character, unsigned int colour, unsigned long x, unsigned long y, uint8_t shadow);