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

#include "gImage.h"
#include "gImage_p.h"
#include "gColor.h"
#include <gConstDefine.h>
#include <gGlobal.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int GImageIO::open ( const string& fileName, const string& mode )
{
    m_fp = fopen ( fileName.c_str(), mode.c_str() );
    return 0;
}

void GImageIO::close()
{
    fclose ( m_fp );
}

GImageIO::GImageIO ( GImage* dst )
{
    m_dst = dst;
    scale_denom = 1;
}

FILE* GImageIO::ioDevice()
{
    return m_fp;
}

GImageIO::~GImageIO()
{
    fclose ( m_fp );
}

class GImageData
{
public:
    int   nbytes;           
    unsigned char **bits;      
    
    string image_format;
    int dirty;
};

GImage::GImage()
    :GPaintDevice(), imgData ( NULL )
{
    initData();
}

GImage::GImage ( const GString& fileName, const string &format )
    :GPaintDevice(), imgData ( NULL )
{
    initData();
    load ( fileName, format );
}

GImage::GImage ( int w, int h, int d ) : GPaintDevice (), imgData ( NULL )
{
    initData();
    create ( w, h, d );
}

GImage::~GImage()
{
    imgData->bits = NULL;
    imgData = NULL;
}

int GImage::load ( GString strPath, const string& format )
{
    if ( imgData->bits )
    {
        imgData->bits = NULL;
        imgData = NULL;
        initData();
    }

    GImageIO io ( this );
    if ( -1 == io.open ( strPath.utf8(), "r" ) )
    {
        return -1;
    }

    if ( format.empty() )
    {
        imgData->image_format = imageFormat ( io.ioDevice() );  
    }
    else
    {
        imgData->image_format = format;
    }

    if ( imgData->image_format.empty() )
    {
        ERROR ( "the file [%s] has a unknown picture format\n", strPath.utf8() );
        return -1;
    }

    GImageHandler *h=NULL;
    h = get_image_handler ( imgData->image_format );
    int ret = 0;
    if ( h && h->read_image )
    {
        rewind ( io.ioDevice() );
        ret = ( *h->read_image ) ( &io );
    }

    return ret;
}

GImage GImage::smoothScale ( int w, int h )
{
    GImageSmoothScaler scaler ( w, h, *this );
    return scaler.scale();
}

string GImage::imageFormat ( FILE* fp )
{
    const int buflen = 14;
    char buf[buflen];
    int item = fread ( buf, 14, 1, fp );
    if ( 1 > item )
    {
        return string();
    }
    string format;
    GImageHandler *p = imageHandlers.first();
    while ( p )
    {
        if ( p->header.match ( buf ) != -1 )
        {
            format = p->format;
            break;
        }
        p = imageHandlers.next();
    }

    return format;
}

GRgb* GImage::scanLine ( const int l ) const
{
    GRgb ( *p ) [][pdevData.w] = reinterpret_cast<GRgb ( * ) [][pdevData.w]> ( imgData->bits );
    return ( *p ) [l];
}

bool GImage::isNull() const
{
    return imgData->bits == NULL;
}

void GImage::initData()
{
    imgData = new GImageData;
    imgData->nbytes = 0;
    imgData->bits = NULL;
    imgData->dirty = 0;
}

void GImage::privRef ( GImageData* sData )
{
    initData();
    imgData->image_format = sData->image_format;
    create ( pdevData.w, pdevData.h, pdevData.d );
    memcpy ( imgData->bits,  sData->bits, pdevData.lineLength*pdevData.h );
}

bool GImage::create ( int w, int h, int d )
{
    int bytes = w*h*d/8;
    unsigned char * buf = allocateMemory ( bytes );
    if ( !buf )
    {
        return false;
    }
    imgData->bits = reinterpret_cast<unsigned char**> ( buf );
    imgData->nbytes = bytes;
    imgData->dirty = 1;
    pdevData.w= w;
    pdevData.h = h;
    pdevData.d = d;
    pdevData.lineLength = w*d>>3;
    initBuf();
    return true;
}

unsigned char* GImage::allocateMemory ( unsigned int bytes )
{
    return ( unsigned char* ) malloc ( bytes );
}

GImage& GImage::operator= ( const GImage& r )
{
    if ( imgData != r.imgData )
    {
        pdevData = r.pdevData;
        imgData->bits = NULL;
        imgData = NULL;
        privRef ( r.imgData );
    }

    return *this;
}

GImage::GImage ( const GImage& src )
    :GPaintDevice ( src ), imgData ( NULL )
{
    privRef ( src.imgData );
}

void GImage::initBuf()
{
    GRgb ( *p ) [][pdevData.w] = reinterpret_cast<GRgb ( * ) [][pdevData.w]> ( imgData->bits );
    GRgb black = gRgb ( 0, 0, 0 );
    
    for ( int line=0; line<pdevData.h; line++ )
    {
        for ( int col=0; col<pdevData.w; col++ )
        {
            ( *p ) [line][col] = black;
        }
    }
}

// nice day ^_^
