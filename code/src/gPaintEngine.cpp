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

#include "gPaintEngine.h"
#include "gUIEvtLoop.h"
#include <gImage.h>
#include <string.h>
#include <gConstDefine.h>
#include <gGlobal.h>

GPaintEngine::GPaintEngine()
{

}

GPaintEngine::~GPaintEngine()
{

}

void GSoftPaintEngine::blt ( const GPoint& dP, const GRect& sR )
{
    GRgb *s = (GRgb*)sDev->scanLine(sR.y());
    GRgb *d = (GRgb*)dDev->scanLine(dP.y());
    s +=  sR.x(), d +=  dP.x();
    
    int w = sR.width(), h = sR.height();
    if( type == IgnoreAlpha)
    {
        copyBlock(s, d, w, h );
        return ;
    }

    blendBlock(s, d, w, h);
}

void GSoftPaintEngine::copyBlock ( unsigned int* s, unsigned int* d, int width, int height )
{
    int nums=width*4;
    for(int i=0; i<height; i++)
    {
        memcpy(d,  s, nums);
        d += dDev->width(),  s += sDev->width();
    }
}

void blendARGB(unsigned char* d, unsigned char *s)
{
    d[3] = 0xFF;
    for(int i=0; i<3; i++)
    {
        d[i] += (s[i]-d[i])*s[3]/255;
    }
}

void GSoftPaintEngine::blendBlock ( unsigned int* s, unsigned int* d, int w, int h )
{
    unsigned char *pD=NULL, *pS=NULL;
    for(int i=0; i<h; i++)
    {
        for(int j=0; j<w; j++)
        {
            pD = (unsigned char*)(d+j);
            pS = (unsigned char*)(s+j);
            blendARGB(pD, pS);
        }
        d += dDev->width(),  s += sDev->width();
    }
}

void GSoftPaintEngine::fillRect ( const GRect& dR )
{
    int w = dR.width(), h = dR.height();
    GRgb color=brush.rgb();

    for(int i=0; i<h; i++)
    {
        GRgb*d = (GRgb*)dDev->scanLine(dR.y()+i );
        d +=  dR.x();
        GRgb (*pos)[w] = reinterpret_cast<GRgb (*)[w]>(d);
        for(int j=0; j<w; j++)
        {
            (*pos)[j] = color;
        }
    }
}

void GPaintEngine::setDestination ( const GPaintDevice* d )

{
    dDev = const_cast<GPaintDevice*> ( d );
}

// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
