// pxCore CopyRight 2005-2006 John Robinson
// Portable Framebuffer and Windowing Library
// pxOffscreenNative.cpp

#include "../pxOffscreen.h"

#include <stdio.h>
#include <stdlib.h>

pxError pxOffscreen::init(int width, int height)
{
    term();

    pxError e = PX_FAIL;

    data = (char*) new unsigned char[width * height * 4];

    if (data)
    {
	setBase(data);
	setWidth(width);
	setHeight(height);
	setStride(width*4);
	setUpsideDown(false);
	e = PX_OK;
    }

    return e;
}

pxError pxOffscreen::term()
{
    return pxOffscreenNative::term();
}

pxError pxOffscreenNative::term()
{
    delete [] data;
    data = NULL;
    
    return PX_OK;
}

void pxOffscreen::blit(pxSurfaceNative s, 
                       int dstLeft, int dstTop, 
                       int w, int h, 
                       int srcLeft, int srcTop)
{
    XImage* image = ::XCreateImage(s->display, 
				   XDefaultVisual(s->display, 
						  XDefaultScreen(s->display)), 
				   24,ZPixmap, 0, (char*)data, 
				   width(), height(), 32, width() * 4);
        
    if (image)
    {
	::XPutImage(s->display, s->drawable, s->gc, image, srcLeft, srcTop, 
		    dstLeft, dstTop, w, h);

	// If we don't NULL this out XDestroyImage will damage
	// the heap by trying to free it internally
	image->data = NULL;
	XDestroyImage(image);
    }
}




