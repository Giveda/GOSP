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

#include "gFontMetrics.h"
#include <stdint.h>
#include <gPainter_p.h>
#include <gConstDefine.h>
#include <gGlobal.h>

class GFontMetricsData
{

public:
    GFont  f;
};

GFontMetrics::GFontMetrics ( const GFont&  f )
    :fmLqH( new GFontMetricsData)
{
    fmLqH->f = f;
}

GFontMetrics::~GFontMetrics()
{
    delete fmLqH;
}

GSize GFontMetrics::size ( int , const GString& str, int , int , int* , char** )
{
    FT_GlyphSlot slot;
    uint16_t result=0, part1=0, part2=0, part3=0;

    FT_Pos w = 0;
    char* pChar = str.utf8();
    FT_Face face = fmLqH->f.face();
    FT_Error err;
    
    while ( *pChar != '\0' )
    {
        if ( ( *pChar&0xf0 ) == 0xe0 )
        {
            part1 = 0x0f & *pChar;
            pChar++;
            part2 = *pChar & 0x3f;
            pChar++;
            part3 = *pChar & 0x3f;
            result = part1<<12 | part2<<6 | part3;
            pChar++;
        }
        else
        {
            result = *pChar;
            pChar++;
        }

        err=FT_Load_Char ( face,  result, FT_LOAD_RENDER | FT_LOAD_NO_BITMAP | FT_LOAD_FORCE_AUTOHINT );
        if ( err )
        {
            ERROR ( "FT_Load_Char err[%d]\n", err );
            continue;
        }
        slot=face->glyph;
        w += ( slot->advance.x>>6 );
    }

    FT_Long  lH = FT_MulFix(face->height, face->size->metrics.y_scale);
    lH >>= 6;
    
    return GSize ( w, lH );
}

int GFontMetrics::ascent ( const GString& str )
{
    FT_GlyphSlot slot;
    uint16_t result=0, part1=0, part2=0, part3=0;

    FT_Pos ascent = 0;
    char* pChar = str.utf8();
    FT_Face face = fmLqH->f.face();
    FT_Error err;
    while ( *pChar != '\0' )
    {
        if ( ( *pChar&0xf0 ) == 0xe0 )
        {
            part1 = 0x0f & *pChar;
            pChar++;
            part2 = *pChar & 0x3f;
            pChar++;
            part3 = *pChar & 0x3f;
            result = part1<<12 | part2<<6 | part3;
            pChar++;
        }
        else
        {
            result = *pChar;
            pChar++;
        }

        err=FT_Load_Char ( face,  result, FT_LOAD_RENDER | FT_LOAD_NO_BITMAP | FT_LOAD_FORCE_AUTOHINT );
        if ( err )
        {
            ERROR ( "FT_Load_Char err[%d]\n", err );
            continue;
        }
        slot=face->glyph;
        ascent = gMax ( ascent, (slot->metrics.horiBearingY) );
    }

    ascent >>= 6;
    return  ascent;
}

GFontMetrics::GFontMetrics ( const GFontMetrics& src)
{
    fmLqH = new GFontMetricsData;
    *fmLqH = *src.fmLqH;
}

GFontMetrics& GFontMetrics::operator= ( const GFontMetrics& r)
{
    if(fmLqH != r.fmLqH)
    {
        *fmLqH = *r.fmLqH;
    }

    return *this;
}

