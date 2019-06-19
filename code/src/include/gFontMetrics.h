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

#ifndef GFONTMETRICS_H
#define GFONTMETRICS_H

#include "gFont.h"
#include "gSize.h"
#include <gConstDefine.h>
#include <gGlobal.h>

class GFontMetricsData;

class GFontMetrics
{
public:
    GFontMetrics( const GFont& f );
    virtual ~GFontMetrics();
    GFontMetrics(const GFontMetrics& );
    GFontMetrics  &operator=(const GFontMetrics  &);

    GSize   size( int flags, const GString& str, int len = -1, int tabstops = 0, int* tabarray = 0, char** intern = 0 );
    int ascent(const GString& str);

private:
    GFontMetricsData   *fmPriv;
};

#endif 


// have a nice day ^_^
