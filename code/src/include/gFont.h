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

#ifndef GFONT_H
#define GFONT_H

#include <gString.h>
#include <gPainter_p.h>
#include <gConstDefine.h>
#include <gGlobal.h>

using namespace std;

struct GFontDef
{
    GString family;
    int     pointSize;
    unsigned int weight;
    bool italic;
};

class GFont
{
public:
    enum Weight
    {
        Light = 25,
        Normal = 50,
        DemiBold = 63,
        Bold  = 75,
        Black  = 87
    };
    GFont();
    GFont(const GFont & src);
    GFont & operator=(const GFont & r);
    virtual ~GFont();
    GFont( const GString &family, int pointSize = 16,
           int weight = Normal, bool italic = false );
    GString family() const {
        return fontData->req.family;
    }
    int pointSize() const {
        return fontData->req.pointSize;
    }
    FT_Face face() const {
        return fontData->m_face;
    }

private:
    struct GFontData
    {
        GFontDef    req;
        FT_Face m_face;
        FT_Error m_err;
        int count;
    } *fontData;

private:
    GString fontDir( const GString& family );
    int getFontFace ( const GFontDef& def );
    void privDeref();
    void privRef();
    void freeFace();
};

#endif 


// reborn, a nice day ^_^
