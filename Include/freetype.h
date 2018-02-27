#ifndef _FF
#define _FF
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <wchar.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
// #define DBG_PRINTF printf
static FT_Library g_tLibrary;
static FT_Face g_tFace;
static FT_GlyphSlot g_tSlot;
static unsigned char *filename = "./Font/msyh.ttc";

int fd_fb;
struct fb_var_screeninfo var; /* Current var */
struct fb_fix_screeninfo fix; /* Current fix */
int screen_size;
unsigned char *fbmem;
unsigned int line_width;
unsigned int pixel_width;

typedef struct FontBitMap
{
    int iXLeft;
    int iYTop;
    int iXMax;
    int iYMax;
    int iBpp;
    int iPitch; /* 对于单色位图, 两行象素之间的跨度 */
    int iCurOriginX;
    int iCurOriginY;
    int iNextOriginX;
    int iNextOriginY;
    unsigned char *pucBuffer;
} T_FontBitMap, *PT_FontBitMap;

void lcd_put_pixel(int x, int y, unsigned int color);
int ShowOneFont(PT_FontBitMap ptFontBitMap);
static int FreeTypeFontInit(char *pcFontFile, unsigned int dwFontSize);
static int FreeTypeGetFontBitmap(unsigned int dwCode, PT_FontBitMap ptFontBitMap);
int PrintString(wchar_t *str, int font_size, int x, int y);
#endif