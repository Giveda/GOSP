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

#ifndef GPAINTENGINE_H
#define GPAINTENGINE_H

#include <gPaintDevice.h>
#include "gPoint.h"
#include "gRect.h"
#include "gBrush.h"
#include <gConstDefine.h>
#include <gGlobal.h>

enum RasterOp
{
    CopyROP,
    OrROP,
    XorROP,
    NotAndROP,
    EraseROP=NotAndROP,
    NotCopyROP,
    NotOrROP,
    NotXorROP,
    AndROP,  NotEraseROP=AndROP,
    NotROP,
    ClearROP,
    SetROP,
    NopROP,
    AndNotROP,
    OrNotROP,
    NandROP,
    NorROP,  LastROP=NorROP
};

enum AlphaType 
{
    IgnoreAlpha, 
    InlineAlpha
};

class GPaintEngine
{
public:
    enum PAINT_TYPE {
        BITBLT=0,
        FILLRECT,
    };
    GPaintEngine();
    virtual ~GPaintEngine();

    void setSource ( const GPaintDevice * d );
    void setDestination ( const GPaintDevice * d );
    void setAlphaType ( AlphaType t );
    void setRop(RasterOp r);

    void setBrush ( const GBrush & b )
    {
        brush = b;
    }

    virtual void blt ( const GPoint &dP,  const GRect &sR ) = 0;
    virtual void fillRect ( const GRect &dR ) = 0;

protected:
    GPaintDevice  *sDev, *dDev;
    AlphaType  type;
    RasterOp  rop;
    GBrush  brush;
};

inline void GPaintEngine::setAlphaType ( AlphaType t )
{
    type = t;
}

inline void GPaintEngine::setSource ( const GPaintDevice* d )
{
    sDev = const_cast<GPaintDevice*> ( d );
}

inline void GPaintEngine::setRop ( RasterOp r )
{
    rop = r;
}

class GSoftPaintEngine : public  GPaintEngine
{
public:
    virtual void blt ( const GPoint &dP,  const GRect &sR );
    virtual void fillRect ( const GRect &dR );

private:
    void copyBlock ( unsigned int* s, unsigned int* d, int width, int height );
    void blendBlock ( unsigned int* s, unsigned int* d, int w, int h );
};

#endif 


// have a nice day ^_^
