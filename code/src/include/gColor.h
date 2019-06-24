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

#ifndef GCOLOR_H
#define GCOLOR_H

#include <gConstDefine.h>
#include <gGlobal.h>


inline GRgb gRed( GRgb rgb )
{
    return (GRgb)((rgb >> 16) & 0xff);
}

inline GRgb gGreen( GRgb rgb )
{
    return (GRgb)((rgb >> 8) & 0xff);
}

inline GRgb gBlue( GRgb rgb )
{
    return (GRgb)(rgb & 0xff);
}

inline GRgb gAlpha( GRgb rgb )
{
    return (GRgb)((rgb >> 24) & 0xff);
}

inline GRgb gRgb( int r, int g, int b )
{
    return (0xff << 24) | ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff);
}

inline GRgb gRgba( int r, int g, int b, int a )
{
    return ((a & 0xff) << 24) | ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff);
}

class GColor
{
public:
    GColor();
    ~GColor();
    GColor( GRgb rgb);
    GColor( int r, int g, int b );
    GRgb  rgb() const
    {
        return m_rgb;
    }

private:
    GRgb   m_rgb;
};

#endif 


