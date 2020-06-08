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

#include <gPainter_p.h>
#include <gConstDefine.h>
#include <gGlobal.h>


FT_Error g_err;

FT_Library g_library;

int init_freetype()
{
    static bool s_b = false;
    if ( s_b )
    {
        return 0;
    }
    s_b = true;

    g_err=FT_Init_FreeType ( &g_library );
    
    if ( g_err )
    {
        ERROR ( "FT_Init_FreeType err[%d]\n", g_err );
        return -10;
    }

    return 0;
}

// a nice day ^_^
// a nice day ^_^
