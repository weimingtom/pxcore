// pxCore CopyRight 2007 John Robinson
// Portable Framebuffer and Windowing Library
// pxOffscreenNative.h

#ifndef PX_OFFSCREEN_NATIVE_H
#define PX_OFFSCREEN_NATIVE_H

#include <windows.h>

#include "../pxCore.h"
#include "../pxBuffer.h"

class pxOffscreenNative: public pxBuffer
{
public:
    pxOffscreenNative(): bitmap(NULL) {}

    HDC beginDrawingWithDC()
    {
        HDC dc = CreateCompatibleDC(NULL);
        savedBitmap = SelectObject(dc, bitmap);
        return dc;
    }

    void endDrawingWithDC(HDC dc)
    {
        SelectObject(dc, savedBitmap);
        DeleteDC(dc);
    }

protected:
    HBITMAP bitmap;

    HGDIOBJ savedBitmap;
};

#endif