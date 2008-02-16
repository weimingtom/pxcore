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
    // Optimization
    if (bitmap)
    {
        if (width == pxBuffer::width() && height == pxBuffer::height())
            return PX_OK;
    }

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

