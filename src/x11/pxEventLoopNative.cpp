// pxCore CopyRight 2005-2006 John Robinson
// Portable Framebuffer and Windowing Library
// pxEventLoopNative.cpp

#include "../pxEventLoop.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysymdef.h>

#include "../pxOffscreen.h"
#include "pxWindowNative.h"

void pxEventLoop::run()
{
    // For now we delegate off to the x11 pxWindowNative class
    pxWindowNative::runEventLoop();
}

void pxEventLoop::exit()
{
    // For now we delegate off to the x11 pxWindowNative class
    pxWindowNative::exitEventLoop();
}


///////////////////////////////////////////
// Entry Point 

int main(int argc, char* argv[])
{
    pxMain();
    return 0;
}
