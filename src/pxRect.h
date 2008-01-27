// pxCore CopyRight 2007 John Robinson
// Portable Framebuffer and Windowing Library
// pxRect.h

#ifndef PX_RECT_H
#define PX_RECT_H

// A class used to describe a rectangle using integer coordinates
class pxRect
{
public:
    pxRect(): mLeft(0), mTop(0), mRight(0), mBottom(0)
    {
    }

    pxRect(int l, int t, int r, int b)
    {
        mLeft = l;
        mTop = t;
        mRight = r;
        mBottom = b;
    }

    int width() const
    {
        return mRight-mLeft;
    }

    int height() const
    {
        return mBottom-mTop;
    }

    int left() const
    {
        return mLeft;
    }

    int top() const
    {
        return mTop;
    }

    int right() const
    {
        return mRight;
    }

    int bottom() const
    {
        return mBottom;
    }

    void setLeft(int l)
    {
        mLeft = l;
    }

    void setTop(int t)
    {
        mTop = t;
    }

    void setBottom(int b)
    {
        mBottom = b;
    }

    void setRight(int r)
    {
        mRight = r;
    }

    void intersect(const pxRect& r)
    {
        mLeft = pxMax<int>(mLeft, r.mLeft);
        mTop = pxMax<int>(mTop, r.mTop);
        mRight = pxMin<int>(mRight, r.mRight);
        mBottom = pxMin<int>(mBottom, r.mBottom);
    }

private:
    int mLeft, mTop, mRight, mBottom;
};


#endif

