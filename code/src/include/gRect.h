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


#ifndef GRECT_H
#define GRECT_H

#include <gSize.h>
#include <gPoint.h>
#include <gConstDefine.h>
#include <gGlobal.h>

#pragma pack(1)
struct T_RECT
{
    T_OFFSET left;
    T_OFFSET top;
    T_OFFSET right;
    T_OFFSET bottom;
};
#pragma pack()

/*! @file  gRect.h
 * @brief  GRect 代表二维平面中的一个矩形区域
 * 
 * @author 明心
 * @version 1.0.0
 * @date 2019-2-6
 */

/*!
 * @class GRect
 * @brief GRect 代表二维平面中的一个矩形区域。对二维平面上的一个矩形区域进行各种数学运算。
 * 
 */
class  /*DLL_PUBLIC*/ GRect
{
public:
    GRect();
    GRect(const GPoint &topleft, const GSize &size);
    GRect(T_OFFSET left, T_OFFSET top, T_OFFSET width, T_OFFSET height);
    GRect(const T_RECT &tr);

    bool isNull() const;
    bool isEmpty() const;
    bool isValid() const;
    
    T_OFFSET left() const;
    T_OFFSET top() const;
    T_OFFSET right() const;
    T_OFFSET bottom() const;

    T_OFFSET x() const;
    T_OFFSET y() const;
    GSize size() const;
    T_OFFSET width() const;
    T_OFFSET height() const;
    void setWidth(T_OFFSET w);
    void setHeight(T_OFFSET h);
    void setSize(const GSize &s);
    void translate(T_OFFSET dx, T_OFFSET dy);
    void translate(const GPoint &p);
    void moveBy(T_OFFSET dx, T_OFFSET dy);
    void moveBy(const GPoint &p);
    void moveTopLeft(const GPoint &p);
    void setRect(T_OFFSET x, T_OFFSET y, T_OFFSET w, T_OFFSET h);
    GRect intersect(const GRect &r) const;
    bool intersects(const GRect &r) const;
    GPoint topLeft() const;
    GPoint bottomLeft() const;
    GPoint topRight() const;
    GPoint bottomRight() const;
    GPoint center() const;
    
    void moveLeft(T_OFFSET pos);
    void moveRight ( T_OFFSET pos );
    void moveTop(T_OFFSET pos);
    void moveBottom ( T_OFFSET pos );
    bool contains(const GRect &r, bool proper=false) const;
    bool contains(const GPoint &p, bool proper=false) const;
    GRect operator&(const GRect &r) const;
    void setLeft(T_OFFSET pos);
    void setTop(T_OFFSET pos);
    void setRight(T_OFFSET pos);
    void setBottom(T_OFFSET pos);
    const T_RECT toT_RECT() const;
    
    friend bool operator==(const GRect &, const GRect &);

private:
    T_RECT m_rectData;
};

#endif // GRECT_H



// have a nice day ^_^
