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

#include "gFont.h"
#include <gConstDefine.h>
#include <gUIEvtLoop.h>
#include <gConstDefine.h>
#include <gGlobal.h>

GFont::GFont()
    :fontData(NULL)
{
    fontData = new GFontData;
    
    fontData->m_face = NULL;
    fontData->req.family = "Sans";
    fontData->req.pointSize = 16;
    fontData->req.weight = Normal;
    fontData->req.italic = false;
    fontData->count = 0;
    getFontFace( fontData->req );
}

GFont::~GFont()
{
    privDeref();
}

GFont::GFont ( const GString& family, int pointSize, int weight, bool italic )
    :fontData(NULL)
{
    fontData = new GFontData;
    fontData->m_face = NULL;
    fontData->req.pointSize = pointSize;
    fontData->req.family = family;
    fontData->req.italic = italic;
    fontData->req.weight = weight;
    fontData->count = 0;
    getFontFace( fontData->req );
}

GString GFont::fontDir( const GString &family )
{
    GString path = GString(uiEvtLp->libPath() )+GString("/fonts/")+family + ".ttf";
    return  path;
}

int  GFont::getFontFace ( const GFontDef & def )
{
    GString strTtf = fontDir( def.family );
    int size = def.pointSize;
    fontData->m_err=FT_New_Face ( g_library, strTtf.utf8(), 0, &fontData->m_face );
    if ( fontData->m_err )
    {
        ERROR ( "FT_New_Face [%s] err[%d]\n", strTtf.utf8(), fontData->m_err );
        return -1;
    }

#if 1
    fontData->m_err=FT_Set_Char_Size (
                        fontData->m_face,    
                        size<<6,       
                        size<<6,   
                        0,     
                        0 );  
    if ( fontData->m_err )
    {
        ERROR ( "FT_Set_Char_Size err[%d]\n", fontData->m_err );
        return -1;
    }
#else
    m_err=FT_Set_Pixel_Sizes (
              m_face,   
              0,      
              16*72 );   
    if ( m_err )
    {
        ERROR ( "FT_Set_Pixel_Sizes err[%d]\n", m_err );
        return -1;
    }
#endif

    return 0;
}

void GFont::privDeref()
{
    fontData->count--;
}

void GFont::privRef()
{
    fontData->count++;
}

void GFont::freeFace()
{
    fontData->m_face = NULL;
}

GFont& GFont::operator= ( const GFont& r )
{
    if(fontData != r.fontData)
    {
        fontData = r.fontData;
    }

    return *this;
}

GFont::GFont ( const GFont& src )
    :fontData(NULL)
{
    if(fontData == src.fontData)
    {
        return ;
    }
    
    fontData = src.fontData;
    
}

// a nice day ^_^
