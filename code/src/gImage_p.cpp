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

#include "gImage_p.h"
#include <gImage.h>
#include <gColor.h>
#include <string.h>
#include <gConstDefine.h>
#include <gGlobal.h>

GImgHList imageHandlers;

GImageHandler::GImageHandler ( const string & h, const string & f, const string& ,
                               image_io_handler w, image_io_handler r )
    : format ( h ), header ( f ), read_image ( w ), write_image ( r )
{

}

void init_image_handlers()
{
    static bool s_b = false;
    if ( s_b )
    {
        return ;
    }
    s_b = true;

    imageHandlers.setAutoDelete ( true );
    
#ifdef CONFIG_SUPPORT_PNG
    extern void zInitPngIO();
    zInitPngIO();
#endif
#ifdef CONFIG_SUPPORT_JPEG
    extern void zInitJpegIO();
    zInitJpegIO();
#endif
}

GImageHandler *get_image_handler ( const string& format )
{
    GImageHandler *p = imageHandlers.first();
    while ( p )            
    {
        if ( p->format == format )
        {
            return p;
        }
        p = imageHandlers.next();
    }
    
    return 0;
}

void   defineIOHandler ( const string& format,
                         const string& header,
                         const string& flags,
                         image_io_handler read_image,
                         image_io_handler write_image )
{
    GImageHandler *p;
    p = new GImageHandler ( format, header, flags,
                            read_image, write_image );
    imageHandlers.append ( p );
}

#ifdef CONFIG_ENABLE_IMAGE_SMOOTHSCALE
class GImageSmoothScalerMhL
{
public:
    int      cols;
    int      newcols;
    int      rows;
    int      newrows;
    bool    hasAlpha;

    const GImage  *src;

    void build ( const int srcWidth, const int srcHeight, const int dstWidth,
                 const int dstHeight, bool hasAlpha );
};

GImageSmoothScaler::GImageSmoothScaler ( const int w, const int h,
        GImage &src )
{
    d = new GImageSmoothScalerMhL;

    d->build ( src.width(), src.height(), w, h, src.hasAlphaBuffer() );
    this->d->src = &src;
}

void
GImageSmoothScalerMhL::build ( const int srcWidth, const int srcHeight,
                                   const int dstWidth, const int dstHeight, bool hasAlphaBuffer )
{
    cols = srcWidth;
    rows = srcHeight;
    newcols = dstWidth;
    newrows = dstHeight;
    hasAlpha = hasAlphaBuffer;
}

int
GImageSmoothScaler::scaledWidth ( void ) const
{
    return d->cols;
}

GImageSmoothScaler::~GImageSmoothScaler ( void )
{
}

inline GRgb*
GImageSmoothScaler::scanLine ( const int line, const GImage *src )
{
    return ( GRgb* ) src->scanLine ( line );
}

GImage
GImageSmoothScaler::scale ( void )
{
    long    SCALE;
    long    HALFSCALE;
    GRgb    *xelrow = 0;
    GRgb    *tempxelrow = 0;
    GRgb    *xP;
    GRgb    *nxP;
    int      row, rowsread;
    int      col, needtoreadrow;
    uint8_t   maxval = 255;
    double  xscale, yscale;
    long    sxscale, syscale;
    long    fracrowtofill, fracrowleft;
    long    *as;
    long    *rs;
    long    *gs;
    long    *bs;
    int      rowswritten = 0;
    GImage  dst;

    if ( d->cols > 4096 )
    {
        SCALE = 4096;
        HALFSCALE = 2048;
    }
    else
    {
        int fac = 4096;
        while ( d->cols * fac > 4096 )
        {
            fac /= 2;
        }

        SCALE = fac * d->cols;
        HALFSCALE = fac * d->cols / 2;
    }

    xscale = ( double ) d->newcols / ( double ) d->cols;
    yscale = ( double ) d->newrows / ( double ) d->rows;
    sxscale = ( long ) ( xscale * SCALE );
    syscale = ( long ) ( yscale * SCALE );

    if ( d->newrows != d->rows ) 
        tempxelrow = new GRgb[d->cols];

    if ( d->hasAlpha )
    {
        as = new long[d->cols];
        for ( col = 0; col < d->cols; ++col )
            as[col] = HALFSCALE;
    }
    else
    {
        as = 0;
    }
    rs = new long[d->cols];
    gs = new long[d->cols];
    bs = new long[d->cols];
    rowsread = 0;
    fracrowleft = syscale;
    needtoreadrow = 1;
    for ( col = 0; col < d->cols; ++col )
        rs[col] = gs[col] = bs[col] = HALFSCALE;
    fracrowtofill = SCALE;

    dst.create ( d->newcols, d->newrows, 32 );
    dst.setAlphaBuffer ( d->hasAlpha );

    for ( row = 0; row < d->newrows; ++row )
    {
        
        if ( d->newrows == d->rows )
        {
            
            tempxelrow = xelrow = scanLine ( rowsread++, d->src );
        }
        else
        {
            while ( fracrowleft < fracrowtofill )
            {
                if ( needtoreadrow && rowsread < d->rows )
                {
                    xelrow = scanLine ( rowsread++, d->src );
                }
                for ( col = 0, xP = xelrow; col < d->cols; ++col, ++xP )
                {
                    if ( as )
                    {
                        as[col] += fracrowleft * gAlpha ( *xP );
                        rs[col] += fracrowleft * gRed ( *xP ) * gAlpha ( *xP ) / 255;
                        gs[col] += fracrowleft * gGreen ( *xP ) * gAlpha ( *xP ) / 255;
                        bs[col] += fracrowleft * gBlue ( *xP ) * gAlpha ( *xP ) / 255;
                    }
                    else
                    {
                        rs[col] += fracrowleft * gRed ( *xP );
                        gs[col] += fracrowleft * gGreen ( *xP );
                        bs[col] += fracrowleft * gBlue ( *xP );
                    }
                }
                fracrowtofill -= fracrowleft;
                fracrowleft = syscale;
                needtoreadrow = 1;
            }
            
            if ( needtoreadrow && rowsread < d->rows )
            {
                xelrow = scanLine ( rowsread++, d->src );
                needtoreadrow = 0;
            }
            for ( col = 0, xP = xelrow, nxP = tempxelrow;
                    col < d->cols; ++col, ++xP, ++nxP )
            {
                register long a, r, g, b;

                if ( as )
                {
                    r = rs[col] + fracrowtofill * gRed ( *xP ) * gAlpha ( *xP ) / 255;
                    g = gs[col] + fracrowtofill * gGreen ( *xP ) * gAlpha ( *xP ) / 255;
                    b = bs[col] + fracrowtofill * gBlue ( *xP ) * gAlpha ( *xP ) / 255;
                    a = as[col] + fracrowtofill * gAlpha ( *xP );
                    if ( a )
                    {
                        r = r * 255 / a * SCALE;
                        g = g * 255 / a * SCALE;
                        b = b * 255 / a * SCALE;
                    }
                }
                else
                {
                    r = rs[col] + fracrowtofill * gRed ( *xP );
                    g = gs[col] + fracrowtofill * gGreen ( *xP );
                    b = bs[col] + fracrowtofill * gBlue ( *xP );
                    a = 0; 
                }
                r /= SCALE;
                if ( r > maxval ) r = maxval;
                g /= SCALE;
                if ( g > maxval ) g = maxval;
                b /= SCALE;
                if ( b > maxval ) b = maxval;
                if ( as )
                {
                    a /= SCALE;
                    if ( a > maxval ) a = maxval;
                    *nxP = gRgba ( ( int ) r, ( int ) g, ( int ) b, ( int ) a );
                    as[col] = HALFSCALE;
                }
                else
                {
                    *nxP = gRgb ( ( int ) r, ( int ) g, ( int ) b );
                }
                rs[col] = gs[col] = bs[col] = HALFSCALE;
            }
            fracrowleft -= fracrowtofill;
            if ( fracrowleft == 0 )
            {
                fracrowleft = syscale;
                needtoreadrow = 1;
            }
            fracrowtofill = SCALE;
        }

        
        if ( d->newcols == d->cols )
        {
            
            memcpy ( dst.scanLine ( rowswritten++ ), tempxelrow, d->newcols*4 );
        }
        else
        {
            register long a, r, g, b;
            register long fraccoltofill, fraccolleft = 0;
            register int needcol;

            nxP = ( GRgb* ) dst.scanLine ( rowswritten++ );
            fraccoltofill = SCALE;
            a = r = g = b = HALFSCALE;
            needcol = 0;
            for ( col = 0, xP = tempxelrow; col < d->cols; ++col, ++xP )
            {
                fraccolleft = sxscale;
                while ( fraccolleft >= fraccoltofill )
                {
                    if ( needcol )
                    {
                        ++nxP;
                        a = r = g = b = HALFSCALE;
                    }
                    if ( as )
                    {
                        r += fraccoltofill * gRed ( *xP ) * gAlpha ( *xP ) / 255;
                        g += fraccoltofill * gGreen ( *xP ) * gAlpha ( *xP ) / 255;
                        b += fraccoltofill * gBlue ( *xP ) * gAlpha ( *xP ) / 255;
                        a += fraccoltofill * gAlpha ( *xP );
                        if ( a )
                        {
                            r = r * 255 / a * SCALE;
                            g = g * 255 / a * SCALE;
                            b = b * 255 / a * SCALE;
                        }
                    }
                    else
                    {
                        r += fraccoltofill * gRed ( *xP );
                        g += fraccoltofill * gGreen ( *xP );
                        b += fraccoltofill * gBlue ( *xP );
                    }
                    r /= SCALE;
                    if ( r > maxval ) r = maxval;
                    g /= SCALE;
                    if ( g > maxval ) g = maxval;
                    b /= SCALE;
                    if ( b > maxval ) b = maxval;
                    if ( as )
                    {
                        a /= SCALE;
                        if ( a > maxval ) a = maxval;
                        *nxP = gRgba ( ( int ) r, ( int ) g, ( int ) b, ( int ) a );
                    }
                    else
                    {
                        *nxP = gRgb ( ( int ) r, ( int ) g, ( int ) b );
                    }
                    fraccolleft -= fraccoltofill;
                    fraccoltofill = SCALE;
                    needcol = 1;
                }
                if ( fraccolleft > 0 )
                {
                    if ( needcol )
                    {
                        ++nxP;
                        a = r = g = b = HALFSCALE;
                        needcol = 0;
                    }
                    if ( as )
                    {
                        a += fraccolleft * gAlpha ( *xP );
                        r += fraccolleft * gRed ( *xP ) * gAlpha ( *xP ) / 255;
                        g += fraccolleft * gGreen ( *xP ) * gAlpha ( *xP ) / 255;
                        b += fraccolleft * gBlue ( *xP ) * gAlpha ( *xP ) / 255;
                    }
                    else
                    {
                        r += fraccolleft * gRed ( *xP );
                        g += fraccolleft * gGreen ( *xP );
                        b += fraccolleft * gBlue ( *xP );
                    }
                    fraccoltofill -= fraccolleft;
                }
            }
            if ( fraccoltofill > 0 )
            {
                --xP;
                if ( as )
                {
                    a += fraccolleft * gAlpha ( *xP );
                    r += fraccoltofill * gRed ( *xP ) * gAlpha ( *xP ) / 255;
                    g += fraccoltofill * gGreen ( *xP ) * gAlpha ( *xP ) / 255;
                    b += fraccoltofill * gBlue ( *xP ) * gAlpha ( *xP ) / 255;
                    if ( a )
                    {
                        r = r * 255 / a * SCALE;
                        g = g * 255 / a * SCALE;
                        b = b * 255 / a * SCALE;
                    }
                }
                else
                {
                    r += fraccoltofill * gRed ( *xP );
                    g += fraccoltofill * gGreen ( *xP );
                    b += fraccoltofill * gBlue ( *xP );
                }
            }
            if ( ! needcol )
            {
                r /= SCALE;
                if ( r > maxval ) r = maxval;
                g /= SCALE;
                if ( g > maxval ) g = maxval;
                b /= SCALE;
                if ( b > maxval ) b = maxval;
                if ( as )
                {
                    a /= SCALE;
                    if ( a > maxval ) a = maxval;
                    *nxP = gRgba ( ( int ) r, ( int ) g, ( int ) b, ( int ) a );
                }
                else
                {
                    *nxP = gRgb ( ( int ) r, ( int ) g, ( int ) b );
                }
            }
        }
    }

    return dst;
    
}
#endif

// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
