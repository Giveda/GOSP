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

#include "gLinuxFB.h"

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/kd.h>
#include <errno.h>
#include <string.h>
#include <gConstDefine.h>
#include <gGlobal.h>

#define OUTPUT_VINFO \
printf("varinfo-xres: %d\n", vinfo.xres); \
printf("varinfo-yres: %d\n", vinfo.yres); \
printf("varinfo-xresvirtual: %d\n", vinfo.xres_virtual); \
printf("varinfo-yresvirtual: %d\n", vinfo.yres_virtual); \
printf("varinfo-xoffset: %d\n", vinfo.xoffset); \
printf("varinfo-yoffset: %d\n", vinfo.yoffset); \
printf("varinfo-bits_per_pixel: %d\n\n", vinfo.bits_per_pixel)
#define OUTPUT_FINFO \
printf("finfo-smem_len: %d\n\n", finfo.smem_len)

void writeTerm ( const char* termctl, int sizeof_termctl )
{
    const char* fileLst[]= {"/dev/tty", "/dev/console", "/dev/tty0", 0};
    const char** dev=fileLst;
    int tty;
    while ( *dev )
    {
        tty=::open ( *dev,O_WRONLY );
        if ( tty>=0 )
        {
            ::write ( tty,termctl,sizeof_termctl );
            ::close ( tty );
        }
        dev++;
    }
}

GLinuxFB* GLinuxFB::instance ( const char* dev )
{
    static  GLinuxFB  s;
    static int ret = s.initDevice ( dev );
    if ( 0 == ret )
    {
        return &s;
    }

    return NULL;
}

GLinuxFB::GLinuxFB()
    :GPaintDevice ()
{

}

GLinuxFB::~GLinuxFB()
{
    shutdownDevice();
}

int GLinuxFB::initDevice ( const char* fb_dev )
{
    static const char termctl[]="\033[9;0]\033[?33l\033[?25l";

    fb_fd = open ( fb_dev,O_RDWR );
    if ( fb_fd<0 )
    {
        ERROR ( "[%s]: [%s]\n", fb_dev, strerror(errno) );
        return -1;
    }

    
    int ret = 0;
    ret = ioctl ( fb_fd, FBIOGET_VSCREENINFO, &vinfo );
    if ( -1 == ret )
    {
        ERROR ( "[%s]: [%s]\n", fb_dev, strerror(errno) );
        goto  ERR_INITDEVICE1;
    }

    OUTPUT_VINFO;

    vinfo.bits_per_pixel = 32;
    
    ret = ioctl ( fb_fd, FBIOPUT_VSCREENINFO, &vinfo );
    if ( -1 == ret )
    {
        ERROR ( "[%s]: [%s]\n", fb_dev, strerror(errno) );
        goto  ERR_INITDEVICE1;
    }

    OUTPUT_VINFO;

    
    ret = ioctl ( fb_fd, FBIOGET_FSCREENINFO, &finfo );
    if ( -1 == ret )
    {
        ERROR ( "[%s]: [%s]\n", fb_dev, strerror(errno) );
        goto  ERR_INITDEVICE1;
    }

    OUTPUT_FINFO;

    fb_base = ( unsigned char * ) mmap ( 0, finfo.smem_len, PROT_READ | PROT_WRITE,
                                         MAP_SHARED, fb_fd, 0 );
    display_base = fb_base + vinfo.yoffset*finfo.line_length + vinfo.xoffset*vinfo.bits_per_pixel/8;
    pdevData.w = vinfo.xres;
    pdevData.h = vinfo.yres;
    pdevData.d = vinfo.bits_per_pixel;
    pdevData.lineLength = finfo.line_length;

    writeTerm ( termctl,sizeof ( termctl ) );

    kdFD = open ( "/dev/tty0", O_RDWR | O_NDELAY, 0 );
    if ( kdFD < 0 )
    {
        ERROR ( "open tty0: [%s]\n",  strerror(errno) );
        return -10;
    }
    ret = ioctl(kdFD, KDGETMODE, &kdMode );
    if(-1 == ret)
    {
        ERROR("ioctl: [%s]\n", strerror(errno) );
    }
    ret = ioctl(kdFD, KDSETMODE, KD_GRAPHICS);
    if(-1 == ret)
    {
        ERROR("ioctl: [%s]\n", strerror(errno) );
    }

    return 0;

ERR_INITDEVICE1:
    close ( fb_fd );
    return -30;
}

void GLinuxFB::shutdownDevice()
{
    int ret = ioctl(kdFD, KDSETMODE, kdMode);
    if(-1 == ret)
    {
        ERROR("ioctl: [%s]\n", strerror(errno) );
    }
    close(kdFD);

    memset( scanLine(0), 0x00,  lineStep()*height() );

    munmap ( ( char* ) fb_base,finfo.smem_len );
    close ( fb_fd );
}

GRgb* GLinuxFB::scanLine ( const int l ) const
{
    GRgb (*p)[][pdevData.w] = reinterpret_cast<GRgb(*)[][pdevData.w]>(display_base);
    return (*p)[l];
}

// have a nice day ^_^
// have a nice day ^_^
