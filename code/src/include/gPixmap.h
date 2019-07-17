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

#ifndef GPIXMAP_H
#define GPIXMAP_H

#include "gImage.h"
#include <gConstDefine.h>
#include <gGlobal.h>

class GPixmap : public  GImage
{
public:
    GPixmap();
    GPixmap( const GString &fn, const string &format=string());
    GPixmap( int w, int h, int d=32 );
    virtual ~GPixmap();
};

#endif 


