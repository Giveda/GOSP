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
#include <gColor.h>
#include <gConstDefine.h>
#include <gGlobal.h>

#include <png.h>

extern "C" {
    
    void giveda_png_io_warning ( png_structp , png_const_charp message );
}


int initGImage ( GImage* image, png_structp png_ptr, png_infop info_ptr )
{
    if ( png_get_valid ( png_ptr, info_ptr, PNG_INFO_gAMA ) )
    {
        double file_gamma;
        png_get_gAMA ( png_ptr, info_ptr, &file_gamma );

        png_set_gamma ( png_ptr, 2.2, file_gamma );
    }

    png_uint_32 width;
    png_uint_32 height;
    int bit_depth;
    png_get_IHDR ( png_ptr, info_ptr, &width, &height, &bit_depth, &bit_depth,
                   0, 0, 0 );

    image->create ( width, height, 32 );
    image->setAlphaBuffer ( true );

    png_read_update_info ( png_ptr, info_ptr );

    return 0;
}

void giveda_png_io_warning ( png_structp , png_const_charp )
{
}

int read_png_image ( GImageIO* iio )
{
    png_structp png_ptr;
    png_infop info_ptr;
    png_infop end_info;

    png_ptr = png_create_read_struct ( PNG_LIBPNG_VER_STRING,0,0,0 );
    if ( !png_ptr )
    {
        return -1;
    }

    png_set_error_fn ( png_ptr, 0, 0, &giveda_png_io_warning );

    info_ptr = png_create_info_struct ( png_ptr );
    if ( !info_ptr )
    {
        png_destroy_read_struct ( &png_ptr, 0, 0 );
        return -2;
    }

    end_info = png_create_info_struct ( png_ptr );
    if ( !end_info )
    {
        png_destroy_read_struct ( &png_ptr, &info_ptr, 0 );
        return -3;
    }

    if ( setjmp ( png_jmpbuf ( png_ptr ) ) )
    {
        png_destroy_read_struct ( &png_ptr, &info_ptr, &end_info );
        return -4;
    }

    png_init_io ( png_ptr, iio->ioDevice() );
    png_set_read_status_fn ( png_ptr, NULL );
    png_read_info ( png_ptr, info_ptr );

    GImage *dst= iio->image();
    int ret = initGImage ( dst, png_ptr, info_ptr );
    if ( 0 > ret )
    {
        png_read_end ( png_ptr, end_info );
        png_destroy_read_struct ( &png_ptr, &info_ptr, &end_info );
        return -7;
    }

    png_uint_32 height=dst->height();
    png_bytep row_buf=NULL;
    for ( png_uint_32 y = 0; y < height; y++ )
    {
        row_buf = ( png_bytep ) dst->scanLine ( y );
        png_read_rows ( png_ptr, ( png_bytepp ) &row_buf, NULL, 1 );
    }

    if ( dst->hasAlphaBuffer() && dst->depth() ==32 )
    {
        GRgb* c;
        int n;
        c = ( GRgb* ) dst->scanLine ( 0 );
        n = dst->width() * dst->height();

        while ( n-- && gAlpha ( *c++ ) ==0xff );
        {
            if ( n<0 ) 
            {
                dst->setAlphaBuffer ( false );
            }
        }
    }

    png_read_end ( png_ptr, end_info );
    png_destroy_read_struct ( &png_ptr, &info_ptr, &end_info );

    return 0;
}

int write_png_image ( GImageIO*  )
{
    NOT_IMP();
    return -1;
}

void zInitPngIO()
{
    defineIOHandler ( "PNG", "^.PNG\r", string(), read_png_image,
                      write_png_image );
    
}

// have a nice day ^_^
