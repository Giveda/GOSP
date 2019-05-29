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

#ifdef CONFIG_gColor

#ifndef GCOLOR_H
#define GCOLOR_H

#include <gGlobal.h>

typedef  uint32_t GRgb;  ///代表一个像素

/**
 * @brief 提取 GRgb 中的红色值
 * 
 * @param rgb color in ARGB8888 format
 * @return GRgb
 */
inline GRgb gRed( GRgb rgb )
{ return (GRgb)((rgb >> 16) & 0xff); }

/**
 * @brief 提取 GRgb 中的绿色值
 * 
 * @param rgb color in ARGB8888 format
 * @return GRgb
 */
 inline GRgb gGreen( GRgb rgb )
{ return (GRgb)((rgb >> 8) & 0xff); }

/**
 * @brief 提取 GRgb 中的蓝色值
 * 
 * @param rgb color in ARGB8888 format
 * @return GRgb
 */
 inline GRgb gBlue( GRgb rgb )
{ return (GRgb)(rgb & 0xff); }

/**
 * @brief 提取 GRgb 中的不透明度
 * 
 * @param rgb color in ARGB8888 format
 * @return GRgb
 */
 inline GRgb gAlpha( GRgb rgb )
{ return (GRgb)((rgb >> 24) & 0xff); }

/**
 * @brief 合成ARGB8888像素，不透明度为0xFF(完全不透明)
 * 
 * @param r 红色值
 * @param g 绿色值
 * @param b 蓝色值
 * @return GRgb ARGB8888像素
 */
inline GRgb gRgb( int r, int g, int b )
{ return (0xff << 24) | ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff); }

/**
 * @brief 合成ARGB8888像素
 * 
 * @param r 红色值
 * @param g 绿色值
 * @param b 蓝色值
 * @param a 不透明度
 * @return GRgb ARGB8888像素
 */
inline GRgb gRgba( int r, int g, int b, int a )
{ return ((a & 0xff) << 24) | ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff); }


/*! @file  gColor.h
 * @brief  GColor 颜色、像素。
 * 
 * @author 明心
 * @version 1.0.0
 * @date 2019-2-4
 */

/*!
 * @class GColor
 * @brief 颜色、像素， GColor 提供了对ARGB8888格式像素的封装
 * 
 */
class DLL_PUBLIC GColor
{
public:
    /**
     * @brief 构造一个red/green/blue均为0，不透明度为0xFF的像素
     * 
     */
    GColor();
    virtual ~GColor();
    
    /**
     * @brief 构造一个值为 GRgb 的像素
     * 
     * @param rgb pixel in ARGB8888 format
     */
    GColor( GRgb rgb );
    
    /**
     * @brief 构造一个格式为ARGB8888的像素
     * 
     * @param r 红色值
     * @param g 绿色值
     * @param b 蓝色值
     */
    GColor( int r, int g, int b );
    
    /**
     * @brief 获取该像素的 GRgb 值
     * 
     * @return GRgb pixel in ARGB8888 format
     */
    GRgb  rgb() const;
    
private:
    GRgb   m_rgb;
};

#endif // GCOLOR_H

#endif  //CONFIG_gColor

