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

#ifndef GBRUSH_H
#define GBRUSH_H

#include <gConstDefine.h>
#include <gGlobal.h>
#include "gColor.h"

enum BrushStyle
{
    NoBrush,
    SolidPattern,
    Dense1Pattern,
    Dense2Pattern,
    Dense3Pattern,
    Dense4Pattern,
    Dense5Pattern,
    Dense6Pattern,
    Dense7Pattern,
    HorPattern,
    VerPattern,
    CrossPattern,
    BDiagPattern,
    FDiagPattern,
    DiagCrossPattern,
    CustomPattern=24
};

class GBrush
{
public:
    GBrush();
    GBrush( const GColor &, BrushStyle=SolidPattern );
    virtual ~GBrush();
    GRgb  rgb()
    {
        return m_color.rgb();
    }

private:
    BrushStyle  m_style;
    GColor   m_color;
};

#endif 


// a nice day ^_^
// a nice day ^_^
