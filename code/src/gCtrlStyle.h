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

#ifndef GCTRLSTYLE_H
#define GCTRLSTYLE_H

#include <gMap.h>
#include <gFont.h>
#include <gPixmap.h>
#include <gColor.h>
#include <gConstDefine.h>
#include <gGlobal.h>

class GCtrlAppStyle;

class GCtrlItemStyle
{
public:
    GCtrlItemStyle()
        :m_pAppStyle ( NULL ) {}
    GCtrlItemStyle ( GCtrlAppStyle* );
    virtual ~GCtrlItemStyle();

    void setPicPath ( const GString& name, const GString& path );
    const GPixmap& pixmap ( const GString& name );

    void setFont ( const GString& name, const GFont& font );
    const GFont font ( const GString& name );

    void setColor ( const GString& name, const GColor& color );
    const GColor color ( const GString& name );

private:
    bool isFileExist ( const GString& strUtf8 );
    int  useResourceFile ( const GString& strPath, const GString& name );
private:
    GMap<GString, GPixmap> m_mapPixmap;
    GMap<GString, GFont> m_mapFont;
    GMap<GString, GColor> m_mapColor;
    GCtrlAppStyle* m_pAppStyle;
};

class GCtrlAppStyle
{
public:
    typedef GMap<GString, GCtrlItemStyle*>  ITEM_STYLE_MAP_T;
    GCtrlAppStyle();
    ~GCtrlAppStyle();
    void appendItemStyle ( const char* className, GCtrlItemStyle* pStyle );
    GCtrlItemStyle* itemStyle ( const char* className );
    const GString libPicPath()
    {
        return m_strPicDir;
    }

private:
    ITEM_STYLE_MAP_T m_dictItemStyle;
    GString m_strPicDir;
};

class GCtrlDefaultAppStyle : public GCtrlAppStyle
{
public:
    GCtrlDefaultAppStyle ( const GString& strUtf8="" )
        :m_strAppPicDir ( strUtf8 )
    {}
    void appendButtonStyle();
    void appendIconViewStyle();
    void appendLineEditStyle();
    void appendListBoxStyle();
    void appendRadioButtonGroupStyle();
    void appendRadioButtonStyle();
    void appendMsgBoxStyle();
    void appendProgressStyle();
    const GString appPicPath()
    {
        return m_strAppPicDir;
    }

private:
    GString m_strAppPicDir;
};

#endif

// a nice day ^_^
