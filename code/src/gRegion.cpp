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

#ifdef CONFIG_gRegion

#ifndef GREGION_H
#define GREGION_H

#include <gRect.h>
#include <gCtrlDllSym.h>

using namespace std;

class GRegionPrivate;

/*! @file  gRegion.h
 * @brief  GRegion 代表二维平面中的一个区域。
 * 
 * @author 明心
 * @version 1.0.0
 * @date 2019-2-6
 */

/*!
 * @class GRegion
 * @brief GRegion 代表二维平面中的一个区域（由若干个矩形区域组成）。对二维平面上的区域进行各种数学运算。
 * 
 */
class DLL_PUBLIC GRegion
{
public:
    /**
     * @brief 构造一个空区域
     * 
     */
    GRegion();
    
    /**
     * @brief 使用指定的矩形去构造一个区域。构造出来的区域和指定的矩形是一样的。
     * 
     * @param r ...
     */
    GRegion( const GRect &r);
    
    /**
     * @brief 拷贝构造函数
     * 
     * @param reg ...
     */
    GRegion(const GRegion &reg);
    
    virtual ~GRegion();
    
    /**
     * @brief 返回本区域和指定区域 r 的交集（相交的区域）
     * 
     * @param r ...
     * @return GRegion
     */
    GRegion   intersect( const GRegion &  r);
    
    /**
     * @brief 返回本区域和指定矩形 r 的交集（相交的区域）
     * 
     * @param r ...
     * @return GRegion
     */
    GRegion   intersect( const GRect &  r);
    
    /**
     * @brief 在二维平面上，将本区域沿X轴平移 x 像素， 沿Y轴平移 y 像素。
     * 
     * @param x ...
     * @param y ...
     * @return void
     */
    void translate ( int x, int y );
    
    /**
     * @brief 返回本区域是否为空
     * 
     * @return bool
     */
    bool isEmpty();
    
    /**
     * @brief 判断本区域是否和 reg 指定的区域相同
     * 
     * @param reg ...
     * @return GRegion&
     */
    GRegion &operator=( const GRegion& reg );
    
    /**
     * @brief 本区域中包含的矩形区域个数
     * 
     * @return uint32_t
     */
    uint32_t size() const;
    
    /**
     * @brief 返回第 i 个矩形
     * 
     * @param i ...
     * @return GRect&
     */
    GRect &at( uint32_t i ) const;
    
    /**
     * @brief 添加矩形到本区域中
     * 
     * @param r ...
     * @return void
     */
    void  append(const GRect & r);
    
private:
    GRegionPrivate *regPriv;
};

#endif // GREGION_H

#endif  //CONFIG_gRegion
