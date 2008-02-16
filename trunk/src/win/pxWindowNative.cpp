// pxCore CopyRight 2007 John Robinson
// Portable Framebuffer and Windowing Library
// pxWindowNative.cpp

#define PX_NATIVE
#include "pxOffscreenNative.h"
#include "pxWindowNative.h"
#include "../pxWindow.h"

#include <tchar.h>
#define _ATL_NO_HOSTING
#include <atlconv.h>

#define WM_DEFERREDCREATE   WM_USER+1000

#ifdef WINCE
#define MOBILE
#include "aygshell.h"
#endif

// pxWindow

pxError pxWindow::init(int left, int top, int width, int height)
{
    HINSTANCE hInstance = ::GetModuleHandle(NULL);

    TCHAR* className = _T("pxWindow");
    WNDCLASS wc;
    if (!::GetClassInfo(hInstance, className, &wc))
    {

	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = (WNDPROC)windowProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = NULL;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = className;

	RegisterClass(&wc);
    }

#ifndef MOBILE
    mWindow = ::CreateWindow(className, _T(""), WS_OVERLAPPEDWINDOW, left, top, width, height, NULL, NULL, 
        hInstance, (pxWindowNative*)this);
#else
    mWindow = CreateWindow(className, L"Hi", WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, (pxWindowNative*)this);

    if (mWindow)
    {
        SHDoneButton (mWindow, SHDB_SHOW);
        SHFullScreen(mWindow, SHFS_HIDESIPBUTTON);
    }
#endif
    return mWindow?PX_OK:PX_FAIL;
}

pxError pxWindow::term()
{
    ::DestroyWindow(mWindow);
    return PX_OK;
}

void pxWindow::invalidateRect(pxRect* r)
{
    RECT wr;
    RECT* pwr = NULL;
    if (r)
    {
        pwr = &wr;
        SetRect(pwr, r->left(), r->top(), r->right(), r->bottom());
    }
    InvalidateRect(mWindow, pwr, FALSE);
}

bool pxWindow::visibility()
{
    return IsWindowVisible(mWindow)?true:false;
}

void pxWindow::setVisibility(bool visible)
{
    ShowWindow(mWindow, visible?SW_SHOW:SW_HIDE);
}

pxError pxWindow::setAnimationFPS(long fps)
{
    if (mTimerId)
    {
        KillTimer(mWindow, mTimerId);
        mTimerId = NULL;
    }

    if (fps > 0)
    {
        mTimerId = SetTimer(mWindow, 1, 1000/fps, NULL);
    }
    return PX_OK;
}

void pxWindow::setTitle(char* title)
{
	USES_CONVERSION;
    ::SetWindowText(mWindow, A2T(title));
}

pxError pxWindow::beginNativeDrawing(pxSurfaceNative& s)
{
    s = ::GetDC(mWindow);
    return s?PX_OK:PX_FAIL;
}

pxError pxWindow::endNativeDrawing(pxSurfaceNative& s)
{
    ::ReleaseDC(mWindow, s);
    return PX_OK;
}

// pxWindowNative

void pxWindowNative::sendSynchronizedMessage(char* messageName, void *p1)
{
    synchronizedMessage m;
    m.messageName = messageName;
    m.p1 = p1;
    ::SendMessage(mWindow, WM_USER, 0, (LPARAM)&m);
}

LRESULT __stdcall pxWindowNative::windowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    int mouseButtonShift = 0;

    if (msg == WM_CREATE)
    {
        if (lParam)
        {
            CREATESTRUCT *cs = (CREATESTRUCT*)lParam;
            ::SetProp(hWnd, _T("wnWindow"), (HANDLE)cs->lpCreateParams);

            // We defer the create notifiation so that timers can be
            // set within the event handler.
            PostMessage(hWnd, WM_DEFERREDCREATE, 0, 0);
        }
    }

    pxWindowNative* w = (pxWindowNative*)::GetProp(hWnd, _T("wnWindow"));

    if (w)
    {
        switch(msg)
        {
// Special case code to handle the "fake" close button in the form
// of an OK button on win mobile
#ifdef MOBILE
        case WM_COMMAND:
			{
            unsigned int wmId    = LOWORD(wParam); 
            unsigned int wmEvent = HIWORD(wParam); 
            // Parse the menu selections:
            switch (wmId)
            {
                case IDOK:
                    SendMessage (w->mWindow, WM_CLOSE, 0, 0);				
                    break;
                default:
                    return DefWindowProc(w->mWindow, msg, wParam, lParam);
            }
			}
            break;
#endif
        case WM_DEFERREDCREATE:
            w->onCreate();
            break;

        case WM_CLOSE:
            w->onCloseRequest(); 
            break;

        case WM_DESTROY:
            w->onClose(); 
            SetProp(hWnd, _T("wnWindow"), NULL);
            break;

        case WM_RBUTTONDOWN: mouseButtonShift++;
        case WM_MBUTTONDOWN: mouseButtonShift++;
        case WM_LBUTTONDOWN:
            {
                SetCapture(w->mWindow);
                unsigned long flags = 1 << mouseButtonShift;
                if (wParam & MK_CONTROL) flags |= PX_MOD_CONTROL;
                if (wParam & MK_SHIFT) flags |= PX_MOD_SHIFT;
                if (GetKeyState(VK_MENU) < 0) flags |= PX_MOD_ALT;

                w->onMouseDown(LOWORD(lParam), HIWORD(lParam), flags);
            }
            break;
        case WM_RBUTTONUP: mouseButtonShift++;
        case WM_MBUTTONUP: mouseButtonShift++;
        case WM_LBUTTONUP:
            {
                ReleaseCapture();
                unsigned long flags = 1 << mouseButtonShift;
                if (wParam & MK_CONTROL) flags |= PX_MOD_CONTROL;
                if (wParam & MK_SHIFT) flags |= PX_MOD_SHIFT;
                if (GetKeyState(VK_MENU) < 0) flags |= PX_MOD_ALT;

                w->onMouseUp(LOWORD(lParam), HIWORD(lParam), flags);
            }
            break;


        case WM_MOUSEMOVE:
            w->onMouseMove((short)LOWORD(lParam), (short)HIWORD(lParam));
            break;

        case WM_TIMER:
            w->onAnimationTimer();
            break;

        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            {
                unsigned long flags = 0;

                if (GetKeyState(VK_SHIFT) & 0x8000)
                {
                    flags |= PX_MOD_SHIFT;
                }
                if (GetKeyState(VK_CONTROL) & 0x8000)
                {
                    flags |= PX_MOD_CONTROL;
                }
                if (GetKeyState(VK_MENU) & 0x8000)
                {
                    flags |= PX_MOD_ALT;
                }

                w->onKeyDown((int)wParam, flags);
            }
            break;

        case WM_KEYUP:
        case WM_SYSKEYUP:
            {
                unsigned long flags = 0;

                if (GetKeyState(VK_SHIFT) & 0x8000)
                {
                    flags |= PX_MOD_SHIFT;
                }
                if (GetKeyState(VK_CONTROL) & 0x8000)
                {
                    flags |= PX_MOD_CONTROL;
                }
                if (GetKeyState(VK_MENU) & 0x8000)
                {
                    flags |= PX_MOD_ALT;
                }

                w->onKeyUp((int)wParam, flags);
            }
            break;
        case WM_PAINT:
            {
                PAINTSTRUCT ps;
                HDC dc = BeginPaint(w->mWindow, &ps);
                w->onDraw(dc);
                EndPaint(w->mWindow, &ps);
            }
            break;
        case WM_SIZE:
            {
                w->onSize(LOWORD(lParam), HIWORD(lParam));
            }
            break;
        case WM_ERASEBKGND:
            return 0;
            break;
        case WM_USER:
            {
                synchronizedMessage *m = (synchronizedMessage*)lParam;
                w->onSynchronizedMessage(m->messageName, m->p1);
            }
            break;
        }
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}
