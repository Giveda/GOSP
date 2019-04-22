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

#ifdef CONFIG_gFontmetrics

#ifndef GFONTMETRICS_H
#define GFONTMETRICS_H

#include <gFont.h>
#include <gSize.h>

class GFontMetricsData;

/*! @file  gFontMetrics.h
 * @brief  GFontMetrics 使用 GFont 来获取文本的显示宽高
 * 
 * @author 明心
 * @version 1.0.0
 * @date 2019-2-6
 */

/*!
 * @class GFontMetrics
 * @brief GFontMetrics 使用 GFont 来获取文本的显示宽高
 * 
 */
class DLL_PUBLIC GFontMetrics
{
public:    
    GFontMetrics( const GFont& f );
    virtual ~GFontMetrics();
    GFontMetrics(const GFontMetrics& );
    GFontMetrics  &operator=(const GFontMetrics  &);
    
    /**
     * @brief 在指定的排版格式下，计算指定文本字符串的显示宽高
     * 
     * @param flags 排版格式
     * @param str 字符串
     * @param len ...
     * @param tabstops ...
     * @param tabarray ...
     * @param intern ...
     * @return GSize
     */
    GSize   size( int flags, const GString& str, int len = -1, int tabstops = 0, int* tabarray = 0, char** intern = 0 );
    
    /**
     * @brief 在限定了显示宽度的情况下，计算指定文本字符串需要的显示高度
     * 
     * @param str 字符串
     * @param lineW 限定显示宽度
     * @return T_OFFSET
     */
    T_OFFSET height(const GString& str, const T_OFFSET lineW);
    
private:
    GFontMetricsData   *fmPriv;
};

#endif // GFONTMETRICS_H

#endif  //CONFIG_gFontmetrics
