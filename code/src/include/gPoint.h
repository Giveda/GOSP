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


#ifndef GPOINT_H
#define GPOINT_H

#include <gConstDefine.h>
#include <gGlobal.h>

#pragma pack(1)
struct T_POINT
{
    T_OFFSET x;
    T_OFFSET y;
};
#pragma pack()

/*! @file  gPoint.h
 * @brief  GPoint 代表二维平面中的一个点
 * 
 * @author 明心
 * @version 1.0.0
 * @date 2019-2-6
 */

/*!
 * @class GPoint
 * @brief GPoint 代表二维平面中的一个点。对二维平面上的一个点进行各种数学运算。
 * 
 */
class  /*DLL_PUBLIC*/ GPoint
{
public:
    GPoint();
    GPoint ( T_OFFSET x, T_OFFSET y );
    GPoint ( const T_POINT& tp );

    T_OFFSET x() const;
    T_OFFSET y() const;
    
    bool isNull() const;
    
    void setX ( T_OFFSET x );
    void setY ( T_OFFSET y );

    T_OFFSET &rx();
    T_OFFSET &ry();

    GPoint &operator+= ( const GPoint & );
    GPoint &operator-= ( const GPoint & );

    GPoint &operator*= ( float  );
    GPoint &operator*= ( int  );
    GPoint &operator*= ( double  );
    
    GPoint &operator/= ( double  );

public:
    friend  bool operator!= ( const GPoint &, const GPoint & );
    friend  const GPoint operator+ ( const GPoint &, const GPoint & );
    friend  const GPoint operator- ( const GPoint &, const GPoint & );
    friend  const GPoint operator- ( const GPoint & );
    friend  const GPoint operator/ ( const GPoint &, double );
    friend  const GPoint operator* ( const GPoint &, float );
    friend  const GPoint operator* ( float, const GPoint & );
    friend  const GPoint operator* ( const GPoint &, double );
    friend  const GPoint operator* ( double, const GPoint & );
    friend  const GPoint operator* ( const GPoint &, int );
    friend  const GPoint operator* ( int, const GPoint & );
    friend  bool operator== ( const GPoint &, const GPoint & );
    
private:
    T_POINT m_pointData;
};

#endif // GPOINT_H



// nice day ^_^
