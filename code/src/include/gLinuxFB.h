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

#ifndef GLINUXFB_H
#define GLINUXFB_H

#include <linux/fb.h>
#include <gPaintDevice.h>
#include <gConstDefine.h>
#include <gGlobal.h>

class GLinuxFB : public GPaintDevice
{
public:
    static  GLinuxFB *instance(const char* dev);
    virtual ~GLinuxFB();
    virtual GRgb* scanLine(const int l) const;

protected:
    GLinuxFB();
    int initDevice( const char* fb_dev );
    void shutdownDevice();

private:
    struct fb_fix_screeninfo finfo;
    struct fb_var_screeninfo vinfo;
    unsigned char *fb_base;
    int fb_fd;
    unsigned char* display_base;
    int kdMode;
    int kdFD;
};

#endif 


// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
