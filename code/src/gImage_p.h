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

#ifndef GIMAGE_P_H
#define GIMAGE_P_H

#include <string>
#include <gRegExp.h>
#include <gPtrList.h>
#include <gConstDefine.h>
#include <gGlobal.h>


using namespace std;

class GImage;
class GImageIO;

typedef int (*image_io_handler)( GImageIO * ); 

class GImageHandler
{
public:
    GImageHandler( const string& f, const string& h, const string& fl, image_io_handler r, image_io_handler w );
    
    string         format;        
    GRegExp       header;        
    image_io_handler  read_image;      
    image_io_handler  write_image;     
};

typedef GPtrList<GImageHandler> GImgHList;
extern  GImgHList imageHandlers;

void init_image_handlers();
void deinit_image_handlers();
GImageHandler *get_image_handler( const string& format );
void   defineIOHandler( const string& format, const string& header, const string& flags, image_io_handler read_image, image_io_handler write_image );

#ifdef CONFIG_ENABLE_IMAGE_SMOOTHSCALE
class GImageSmoothScalerHmq;
class GImageSmoothScaler
{
public:
    GImageSmoothScaler( const int w, const int h, GImage& src );
    GImageSmoothScaler(const int srcWidth, const int srcHeight,
                       const char *parameters);
    virtual ~GImageSmoothScaler(void);

    GImage  scale();

protected:
    int scaledWidth(void) const;

private:
    GImageSmoothScalerHmq  *d;
    virtual GRgb* scanLine(const int line = 0, const GImage *src = 0);
};

#endif

#endif 

// nice day ^_^
// for fun ^_^
