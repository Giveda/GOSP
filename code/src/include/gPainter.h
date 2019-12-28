/*
 * Copyright (C) 2019  明心  <imleizhang@qq.com>
 * All rights reserved.
 *
 * This program is an open-source software; and it is distributed in the hope
 * that it will be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.
 * This program is not a free software; so you can not redistribute it(include
 * binary form and source code form) without my authorization. And if you
 * need use it for any commercial purpose, you should first get commercial
 * authorization from me, otherwise there will be your's legal&credit risks.
 *
 */

#ifndef GPAINTER_H
#define GPAINTER_H

#include <gPixmap.h>
#include <gImage.h>
#include <gRect.h>
#include <gColor.h>
#include <gFont.h>
#include <gBrush.h>
#include <gPaintEngine.h>
#include <gConstDefine.h>
#include <gGlobal.h>

class GPainterSelf;
class GPaintDevice;

class GPainter
{
    G_DISABLE_COPY_1(GPainter)
public:
    GPainter();
    explicit GPainter( GPaintDevice* p );
    virtual ~GPainter();
    GPaintDevice *device() const;
    void setDevice(GPaintDevice *p);
    void drawPixmap ( const GPixmap& pix, const GRect &dR, const GRect &sR );
    void drawPixmap (int dx, int dy, const GPixmap &pix);
    void drawPixmap ( int dx, int dy, const GPixmap &pix,  int sx, int sy, int sw, int sh );
    void save();
    void restore();
    void translate(int x, int y);
    void drawImage ( int dx, int dy, const GImage &img, int sx, int sy, int sw, int sh );
    void setPen ( const GColor& c );
    void setFont ( const GFont& f );
    void setBrush ( const GBrush &b );
    void drawText ( int dX, int dY, int dW, int dH, int flags, const GString& str );
    void fillRect(int x, int y, int w, int h, const GBrush &b);
    void fillRect( const GRect & r, const GBrush &b);
    void fillRect( const GRect & r);
    static GPixmap grabWidget ( GPaintDevice* dev, const GRect& dR );
    void bitBlt ( GPaintDevice* dst, GPoint dP, GPaintDevice* src, GRect sR, RasterOp rop, bool noBlend=false );

private:
    void drawTextSelf ( GRect dR, const GString& str, int flags );
    GPainterSelf *m_priv;
};

void bitBlt ( GPaintDevice* dst, GPoint dP, GPaintDevice* src, GRect sR, RasterOp rop, bool noBlend=false );

#endif 


// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
