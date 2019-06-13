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

#ifndef GIMAGE_H
#define GIMAGE_H

#include "gString.h"
#include <gPaintDevice.h>
#include <gShared.h>
#include <gConstDefine.h>
#include <gGlobal.h>

class GImageData;
class GImage;

class GImageIO
{
public:
    GImageIO(GImage* dst);
    virtual ~GImageIO();
    int  open ( const string& fileName, const string& mode );
    void close();
    FILE* ioDevice();
    GImage* image() {
        return m_dst;
    }
    unsigned int scale_denom;

private:
    FILE *m_fp;
    GImage *m_dst;
};

class GImage : public GPaintDevice
{
public:
    friend class GPaintEngine;
    GImage();
    GImage( const GString &fileName, const string &format=string() );
    GImage( int w, int h, int d=32 );
    GImage(const GImage & src);
    GImage & operator=(const GImage & r);
    virtual ~GImage();

    bool create ( int w, int h, int d=32 );
    int load ( GString strPath, const string &format=string() );
    bool isNull() const;
    GImage smoothScale ( int width, int height );
    virtual GRgb* scanLine ( const int l ) const;

private:
    string imageFormat ( FILE* fp );
    void initData();
    void privRef(GImageData  *sData);
    void initBuf();
    virtual unsigned char * allocateMemory(unsigned int  bytes);

private:
    GImageData  *imgData;
};


#endif 


// have a nice day ^_^
// have a nice day ^_^
