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

#include "gPoint.h"
#include <netinet/in.h>
#include <gConstDefine.h>
#include <gGlobal.h>

/*****************************************************************************
 *  GPoint functions
 *****************************************************************************/

GPoint::GPoint()
{
    m_pointData.x=0;
    m_pointData.y=0;
}

GPoint::GPoint ( T_OFFSET x,  T_OFFSET y )
{
    m_pointData.x = x;
    m_pointData.y = y;
}

bool GPoint::isNull() const
{
    return m_pointData.x == 0 && m_pointData.y == 0;
}

T_OFFSET GPoint::x() const
{
    return m_pointData.x;
}

T_OFFSET GPoint::y() const
{
    return m_pointData.y;
}

void GPoint::setX ( T_OFFSET x )
{
    m_pointData.x = x;
}

void GPoint::setY ( T_OFFSET y )
{
    m_pointData.y = y;
}

T_OFFSET &GPoint::rx()
{
    return m_pointData.x;
}

T_OFFSET &GPoint::ry()
{
    return m_pointData.y;
}

GPoint &GPoint::operator+= ( const GPoint &p )
{
    m_pointData.x+=p.m_pointData.x;
    m_pointData.y+=p.m_pointData.y;
    return *this;
}

GPoint &GPoint::operator-= ( const GPoint &p )
{
    m_pointData.x-=p.m_pointData.x;
    m_pointData.y-=p.m_pointData.y;
    return *this;
}

GPoint &GPoint::operator*= ( float c )
{
    m_pointData.x = gRound ( m_pointData.x*c );
    m_pointData.y = gRound ( m_pointData.y*c );
    return *this;
}

GPoint &GPoint::operator*= ( double c )
{
    m_pointData.x = gRound ( m_pointData.x*c );
    m_pointData.y = gRound ( m_pointData.y*c );
    return *this;
}

GPoint &GPoint::operator*= ( int c )
{
    m_pointData.x = m_pointData.x*c;
    m_pointData.y = m_pointData.y*c;
    return *this;
}

GPoint &GPoint::operator/= ( double c )
{
    m_pointData.x = gRound ( m_pointData.x/c );
    m_pointData.y = gRound ( m_pointData.y/c );
    return *this;
}

GPoint::GPoint(const T_POINT& tp)
{
    m_pointData.x = ntohs(tp.x);
    m_pointData.y = ntohs(tp.y);
}

bool operator== ( const GPoint &p1, const GPoint &p2 )
{
    return p1.m_pointData.x == p2.m_pointData.x && p1.m_pointData.y == p2.m_pointData.y;
}

bool operator!= ( const GPoint &p1, const GPoint &p2 )
{
    return p1.m_pointData.x != p2.m_pointData.x || p1.m_pointData.y != p2.m_pointData.y;
}

const GPoint operator+ ( const GPoint &p1, const GPoint &p2 )
{
    return GPoint ( p1.m_pointData.x+p2.m_pointData.x, p1.m_pointData.y+p2.m_pointData.y );
}

const GPoint operator- ( const GPoint &p1, const GPoint &p2 )
{
    return GPoint ( p1.m_pointData.x-p2.m_pointData.x, p1.m_pointData.y-p2.m_pointData.y );
}

const GPoint operator- ( const GPoint &p )
{
    return GPoint ( -p.m_pointData.x, -p.m_pointData.y );
}

const GPoint operator* ( const GPoint &p, float c )
{
    return GPoint ( gRound ( p.m_pointData.x*c ), gRound ( p.m_pointData.y*c ) );
}

const GPoint operator* ( const GPoint &p, double c )
{
    return GPoint ( gRound ( p.m_pointData.x*c ), gRound ( p.m_pointData.y*c ) );
}

const GPoint operator* ( const GPoint &p, int c )
{
    return GPoint ( p.m_pointData.x*c, p.m_pointData.y*c );
}

const GPoint operator* ( float c, const GPoint &p )
{
    return GPoint ( gRound ( p.m_pointData.x*c ), gRound ( p.m_pointData.y*c ) );
}

const GPoint operator* ( double c, const GPoint &p )
{
    return GPoint ( gRound ( p.m_pointData.x*c ), gRound ( p.m_pointData.y*c ) );
}

const GPoint operator* ( int c, const GPoint &p )
{
    return GPoint ( p.m_pointData.x*c, p.m_pointData.y*c );
}

const GPoint operator/ ( const GPoint &p, double c )
{
    return GPoint ( gRound ( p.m_pointData.x/c ), gRound ( p.m_pointData.y/c ) );
}
// have a nice day
// have a nice day
