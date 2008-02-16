// pxCore CopyRight 2007 John Robinson
// Portable Framebuffer and Windowing Library
// pxBufferNative.cpp

#include "../pxCore.h"
#include "pxBufferNative.h"
#include "../pxRect.h"

#include "../pxOffscreen.h"

void pxBuffer::blit(pxSurfaceNative s, int dstLeft, int dstTop, int dstWidth, int dstHeight, 
    int srcLeft, int srcTop, int srcWidth, int srcHeight)
{
	
	if (!upsideDown())
	{
		Rect pr;
		MacSetRect(&pr, 0, 0, width(), height());	
		
		GWorldPtr gworld;
		NewGWorldFromPtr (&gworld, 32, &pr, NULL, NULL, 0, (char*)base(), 4*width());

		Rect dr, sr;
		MacSetRect(&dr, dstLeft, dstTop, dstLeft + dstWidth, dstTop + dstHeight);
		MacSetRect(&sr, srcLeft, srcTop, srcLeft + srcWidth, srcTop + srcHeight);
		CopyBits((BitMapPtr)*GetGWorldPixMap(gworld),(BitMapPtr)*GetGWorldPixMap(s),&sr,&dr,srcCopy,NULL);
	
		DisposeGWorld(gworld);
	}
	else 
	{
		pxRect r(srcLeft, srcTop, srcLeft+srcWidth, srcTop+srcHeight);
		r.intersect(bounds());
		
		pxOffscreen flipped;
		flipped.init(r.width(), r.height());

		// flip it
		
		for (int y = 0; y < flipped.height(); y++)
		{
			pxPixel *s = pixel(r.left(), r.top()+y);
			pxPixel *se = s + r.width();
			pxPixel *d = flipped.scanline(y);
			while (s < se)
			{
				*d++ = *s++;
			}
		}
		
		Rect pr;
		MacSetRect(&pr, 0, 0, flipped.width(), flipped.height());	
		
		GWorldPtr gworld;
		NewGWorldFromPtr (&gworld, 32, &pr, NULL, NULL, 0, (char*)flipped.base(), 4*flipped.width());

		Rect dr, sr;
		MacSetRect(&dr, dstLeft, dstTop, dstLeft + dstWidth, dstTop + dstHeight);
		MacSetRect(&sr, srcLeft-r.left(), srcTop-r.top(), 
			srcLeft-r.left()+srcWidth, srcTop-r.top()+srcHeight);
		CopyBits((BitMapPtr)*GetGWorldPixMap(gworld),(BitMapPtr)*GetGWorldPixMap(s),&sr,&dr,srcCopy,NULL);
	
		DisposeGWorld(gworld);
	}

}
