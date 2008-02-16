// pxCore CopyRight 2007 John Robinson
// Portable Framebuffer and Windowing Library
// pxBufferNative.cpp

#include "../pxCore.h"
#include "pxBufferNative.h"
#include "../pxRect.h"

void pxBuffer::blit(pxSurfaceNative s, int dstLeft, int dstTop, int dstWidth, int dstHeight, 
    int srcLeft, int srcTop, int srcWidth, int srcHeight)
{
    int h = (upsideDown()?1:-1) * height();
    BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER), width(), h, 1, 32 } };  

#ifndef WINCE
    ::SetStretchBltMode(s, COLORONCOLOR);
#endif
    ::StretchDIBits(
        s,            // handle of device context 
        srcLeft,           // x-coordinate of upper-left corner of source rect. 
        srcTop,           // y-coordinate of upper-left corner of source rect. 
        srcWidth,       // width of source rectangle 
        srcHeight,      // height of source rectangle 
        dstLeft,
        dstTop,           // x, y -coordinates of upper-left corner of dest. rect. 
        dstWidth,       // width of destination rectangle 
        dstHeight,      // height of destination rectangle 
        base(),           // address of bitmap bits 
        &bi,           // address of bitmap data 
        DIB_RGB_COLORS,  // usage 
        SRCCOPY          // raster operation code 
    );
}
