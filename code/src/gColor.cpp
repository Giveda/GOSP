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

#ifdef CONFIG_gBrush

#ifndef GBRUSH_H
#define GBRUSH_H

#include <gColor.h>

///这个enum用于索引画刷风格
enum BrushStyle
{
    ///无画刷
    NoBrush,
    
    ///实心画刷
    SolidPattern,
};

#pragma pack(1)
struct T_BRUSH
{
    GRgb rgb;
    BrushStyle  style;
};
#pragma pack()

/*! @file  gBrush.h
 * @brief  GBrush 画刷，用于在绘制时指定填充风格。
 * 
 * @author 明心
 * @version 1.0.0
 * @date 2019-2-4
 */

/*!
 * @class GBrush
 * @brief 画刷，用于在绘制时指定填充风格。默认、和最常见的就是实心填充风格 SolidPattern （能够实现实心填充）
 * 
 */
class DLL_PUBLIC GBrush
{
public:
    /**
     * @brief 使用默认的 GColor 构造一个实心填充风格的画刷
     * 
     */
    GBrush();
    

    /**
     * @brief 使用指定的 GColor 构造一个指定填充风格的画刷
     * 
     * @param color 指定的 GColor
     * @param style 指定的  BrushStyle ，默认为实心画刷
     */
    GBrush( const GColor & color, BrushStyle style=SolidPattern );
    
    GBrush( const T_BRUSH& tb);
    virtual ~GBrush();
    
    /**
     * @brief 返回本画刷的色彩 GRgb 
     * 
     * @return GRgb
     */
    GRgb  rgb();
    
    /**
     * @brief 返回本画刷的填充风格 BrushStyle 
     * 
     * @return BrushStyle
     */
    BrushStyle style();
    
    const T_BRUSH toT_BRUSH() const;
    
private:
    T_BRUSH  brushData;
};

#endif // GBRUSH_H

#endif  //CONFIG_gBrush

