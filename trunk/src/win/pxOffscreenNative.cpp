// pxCore CopyRight 2007 John Robinson
// Portable Framebuffer and Windowing Library
// pxOffscreenNative.cpp

#define PX_NATIVE

#include "../pxCore.h"
#include "pxOffscreenNative.h"
#include "../pxOffscreen.h"
#include "../pxRect.h"

pxError pxOffscreen::init(int width, int height)
{
    term();  // release all resources if this object is reinitialized

    void *base;
    BITMAPINFO bmpInfo = { { sizeof(BITMAPINFOHEADER), width, height, 1, 32 } };            
    
    bitmap = CreateDIBSection(NULL, (BITMAPINFO*)&bmpInfo, DIB_RGB_COLORS, 
                                (void **)&base, NULL, NULL);

    
    setBase(base);
    setWidth(width);
    setHeight(height);
    setStride(width*4);
    setUpsideDown(true);
    
    return bitmap?PX_OK:PX_FAIL;
}

pxError pxOffscreen::term()
{
    if (bitmap)
    {
	DeleteObject(bitmap);
        setBase(NULL);
    }

	return PX_OK;
}

void pxOffscreen::blit(pxSurfaceNative s, int dstLeft, int dstTop, int width, int height, 
    int srcLeft, int srcTop)
{
    HDC screenDC = ::GetWindowDC(NULL);
    HDC drawingDC = ::CreateCompatibleDC(screenDC);
    HGDIOBJ oldBM = SelectObject(drawingDC, bitmap);

    
    ::BitBlt(s, dstLeft, dstTop, width, height, drawingDC, srcLeft, srcTop,
            SRCCOPY);

    SelectObject(drawingDC, oldBM);
    DeleteDC(drawingDC);
    ReleaseDC(NULL, screenDC);
}
