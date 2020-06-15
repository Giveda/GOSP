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

#ifndef GPAINTDEVICE_H
#define GPAINTDEVICE_H

#include <gSize.h>
#include <gConstDefine.h>
#include <gGlobal.h>

class GPaintEngine;

class GPaintDevice
{
public:
    friend class GPaintEngine;
    GPaintDevice( );
    virtual ~GPaintDevice();

    virtual GRgb* scanLine(const int l) const = 0;
    int width () const
    {
        return pdevData.w;
    }
    int height () const
    {
        return pdevData.h;
    }
    int depth() const
    {
        return pdevData.d;
    }
    GSize size() const
    {
        return GSize ( pdevData.w, pdevData.h );
    }
    int lineStep() const
    {
        return pdevData.lineLength;
    }
    bool hasAlphaBuffer();
    void setAlphaBuffer ( bool alpha );

protected:
    struct GPaintDeviceData
    {
        int   w;
        int   h;
        int   d;
        int   lineLength;
        bool  alpha;
    } pdevData;
};

#endif 


// nice day ^_^
