/*
 * Copyright (C) 2019  明心  <imleizhang@qq.com>
 * All rights reserved.
 * 
 * This program is an open-source software; and it is distributed in the hope 
 * that it will be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE. 
 * This program is not a free software; so you can not redistribute it and/or 
 * modify it without my authorization. If you only need use it for personal
 * study purpose(no redistribution, and without any  commercial behavior), 
 * you should accept and follow the GNU AGPL v3 license, otherwise there
 * will be your's credit and legal risks.  And if you need use it for any 
 * commercial purpose, you should first get commercial authorization from
 * me, otherwise there will be your's credit and legal risks. 
 *
 */

#include <config_giveda.h>

#ifdef CONFIG_gFont

#ifndef GFONT_H
#define GFONT_H

#include <gString.h>
#include <gSharedData.h>
#include <gCtrlDllSym.h>

using namespace std;

class GFontData;

/*! @file  gFont.h
 * @brief  GFont 字体、字号等信息
 * 
 * @author 明心
 * @version 1.0.0
 * @date 2019-2-6
 */

/*!
 * @class GFont
 * @brief 字体、字号等信息。
 * 
 */
class DLL_PUBLIC GFont
{
public:
    enum Weight 
    {
        Light = 25, 
        Normal = 50, 
        DemiBold = 63,
        Bold  = 75, 
        Black  = 87, 
    };
    enum PointSize 
    {
        PointSize_24=24,
        PointSize_28=28,
        PointSize_32=32,
        PointSize_36=36,
    };
    GFont();
    GFont(const GFont & src);
    GFont & operator=(const GFont & r);
    virtual ~GFont();
    
    /**
     * @brief 使用指定的信息构造一个字体对象
     * 
     * @param family 字体名称
     * @param pointSize 字号
     * @param weight 加粗
     * @param italic 斜体
     */
    GFont( const GString &family, PointSize pointSize = PointSize_24,
           Weight weight = Normal, bool italic = false );
    
    /**
    * @brief 获取字体名称
    * 
    * @return const GString&
    */
    const GString& family() const;
    
    /**
     * @brief 获取字体大小
     * 
     * @return GFont::PointSize
     */
    PointSize pointSize() const;
    
    /**
     * @brief 获取weight
     * 
     * @return GFont::Weight
     */
    Weight weight() const;
    
    /**
     * @brief 是否是斜体
     * 
     * @return bool
     */
    bool italic() const;
    
private:
    GFontData *fontData;
    
private:
    DLL_LOCAL void privDeref();
    DLL_LOCAL void privRef();
};

#endif // GFONT_H

#endif  //CONFIG_gFont
