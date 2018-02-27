#include "freetype.h"

/* color : 0x00RRGGBB */
void lcd_put_pixel(int x, int y, unsigned int color)
{
    unsigned char *pen_8 = fbmem + y * line_width + x * pixel_width;
    unsigned short *pen_16;
    unsigned int *pen_32;

    unsigned int red, green, blue;

    pen_16 = (unsigned short *)pen_8;
    pen_32 = (unsigned int *)pen_8;
    //  printf("pen_8=%d\n",pen_8);
    // printf("var.bits_per_pixel=%d\n",var.bits_per_pixel);
    switch (var.bits_per_pixel)
    {
    case 8:
    {
        *pen_8 = color;
        break;
    }
    case 16:
    {
        /* 565 */
        red = (color >> 16) & 0xff;
        green = (color >> 8) & 0xff;
        blue = (color >> 0) & 0xff;
        color = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
        *pen_16 = color;
        break;
    }
    case 32:
    {
        *pen_32 = color;
        break;
    }
    default:
    {
        printf("can't surport %dbpp\n", var.bits_per_pixel);
        break;
    }
    }
}

static int FreeTypeGetFontBitmap(unsigned int dwCode, PT_FontBitMap ptFontBitMap)
{
    int iError;
    int iPenX = ptFontBitMap->iCurOriginX;
    int iPenY = ptFontBitMap->iCurOriginY;

    /* load glyph image into the slot (erase previous one) */
    //iError = FT_Load_Char(g_tFace, dwCode, FT_LOAD_RENDER );
    iError = FT_Load_Char(g_tFace, dwCode, FT_LOAD_RENDER | FT_LOAD_MONOCHROME);

    //DBG_PRINTF("iPenX = %d, iPenY = %d, bitmap_left = %d, bitmap_top = %d, width = %d, rows = %d\n", iPenX, iPenY, g_tSlot->bitmap_left, g_tSlot->bitmap_top, g_tSlot->bitmap.width, g_tSlot->bitmap.rows);
    ptFontBitMap->iXLeft = iPenX + g_tSlot->bitmap_left;
    ptFontBitMap->iYTop = iPenY - g_tSlot->bitmap_top;
    ptFontBitMap->iXMax = ptFontBitMap->iXLeft + g_tSlot->bitmap.width;
    ptFontBitMap->iYMax = ptFontBitMap->iYTop + g_tSlot->bitmap.rows;
    ptFontBitMap->iBpp = 1;
    ptFontBitMap->iPitch = g_tSlot->bitmap.pitch;
    ptFontBitMap->pucBuffer = g_tSlot->bitmap.buffer;

    ptFontBitMap->iNextOriginX = iPenX + g_tSlot->advance.x / 64;
    ptFontBitMap->iNextOriginY = iPenY;

    return 0;
}

int ShowOneFont(PT_FontBitMap ptFontBitMap)
{
    int x;
    int y;
    unsigned char ucByte = 0;
    int i = 0;
    int bit;
    if (ptFontBitMap->iBpp == 1)
    {
        for (y = ptFontBitMap->iYTop; y < ptFontBitMap->iYMax; y++)
        {
            i = (y - ptFontBitMap->iYTop) * ptFontBitMap->iPitch;
            for (x = ptFontBitMap->iXLeft, bit = 7; x < ptFontBitMap->iXMax; x++)
            {
                if (bit == 7)
                {
                    ucByte = ptFontBitMap->pucBuffer[i++];
                }

                if (ucByte & (1 << bit))
                {
                    lcd_put_pixel(x, y, 0xffffffff);
                    //g_ptDispOpr->ShowPixel(x, y, COLOR_FOREGROUND);
                }
                else
                {
                    /* 使用背景色, 不用描画 */
                    // g_ptDispOpr->ShowPixel(x, y, 0); /* 黑 */
                }
                bit--;
                if (bit == -1)
                {
                    bit = 7;
                }
            }
        }
    }
    else
    {
        printf("ShowOneFont error, can't support %d bpp\n", ptFontBitMap->iBpp);
        return -1;
    }
    return 0;
}
static int FreeTypeFontInit(char *pcFontFile, unsigned int dwFontSize)
{
    int iError;
    /* 显示矢量字体 */
    iError = FT_Init_FreeType(&g_tLibrary);                    /* 初始化库 */
    iError = FT_New_Face(g_tLibrary, pcFontFile, 0, &g_tFace); /*根据字体文件得到face */
    g_tSlot = g_tFace->glyph;
    iError = FT_Set_Pixel_Sizes(g_tFace, dwFontSize, 0); /* 设置字体大小*/
    return 0;
}

int PrintString(wchar_t *str, int font_size, int x, int y)
{
    int i;
    int n;
    int error;
    T_FontBitMap tFontBitMap;

    tFontBitMap.iCurOriginX = x;
    tFontBitMap.iCurOriginY = y;
    fd_fb = open("/dev/fb0", O_RDWR);
    if (fd_fb < 0)
    {
        printf("can't open /dev/fb0\n");
        return -1;
    }
    if (ioctl(fd_fb, FBIOGET_VSCREENINFO, &var))
    {
        printf("can't get var\n");
        return -1;
    }
    if (ioctl(fd_fb, FBIOGET_FSCREENINFO, &fix))
    {
        printf("can't get fix\n");
        return -1;
    }
    line_width = var.xres * var.bits_per_pixel / 8;
    // printf("line_width=%d\n", line_width);
    pixel_width = var.bits_per_pixel / 8;
    // printf("pixel_width=%d\n", pixel_width);
    screen_size = var.xres * var.yres * var.bits_per_pixel / 8;
    // printf("screen_size=%d\n", screen_size);
    fbmem = (unsigned char *)mmap(NULL, screen_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_fb, 0);
    error = FreeTypeFontInit(filename, font_size);
    for (n = 0; n < wcslen(str); n++)
    {
        FreeTypeGetFontBitmap(str[n], &tFontBitMap);
        ShowOneFont(&tFontBitMap);
        tFontBitMap.iCurOriginX = tFontBitMap.iNextOriginX;
        tFontBitMap.iCurOriginY = tFontBitMap.iNextOriginY;
    }

    return 0;
}