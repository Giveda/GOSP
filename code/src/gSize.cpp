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

#include "gSize.h"
#include <gConstDefine.h>
#include <gGlobal.h>

/*****************************************************************************
 *  GSize functions
 *****************************************************************************/

GSize::GSize()
{
    m_data.w = m_data.h = -1;
    
}

GSize::GSize ( T_OFFSET w, T_OFFSET h )
{
    m_data.w = w;
    m_data.h = h;
}

bool GSize::isNull() const
{
    return m_data.w==0 && m_data.h==0;
}

bool GSize::isEmpty() const
{
    return m_data.w<1 || m_data.h<1;
}

bool GSize::isValid() const
{
    return m_data.w>=0 && m_data.h>=0;
}

T_OFFSET GSize::width() const
{
    return m_data.w;
}

T_OFFSET GSize::height() const
{
    return m_data.h;
}

void GSize::setWidth ( T_OFFSET w )
{
    m_data.w = w;
}

void GSize::setHeight ( T_OFFSET h )
{
    m_data.h = h;
}


T_OFFSET &GSize::rwidth()
{
    return m_data.w;
}

T_OFFSET &GSize::rheight()
{
    return m_data.h;
}

GSize &GSize::operator+= ( const GSize &s )
{
    m_data.w+=s.m_data.w;
    m_data.h+=s.m_data.h;
    return *this;
}

GSize &GSize::operator-= ( const GSize &s )
{
    m_data.w-=s.m_data.w;
    m_data.h-=s.m_data.h;
    return *this;
}

GSize &GSize::operator*= ( double c )
{
    m_data.w = gRound ( m_data.w*c );
    m_data.h = gRound ( m_data.h*c );
    return *this;
}

/*DLL_PUBLIC*/ bool operator== ( const GSize &s1, const GSize &s2 )
{
    return s1.m_data.w == s2.m_data.w && s1.m_data.h == s2.m_data.h;
}

/*DLL_PUBLIC*/ bool operator!= ( const GSize &s1, const GSize &s2 )
{
    return s1.m_data.w != s2.m_data.w || s1.m_data.h != s2.m_data.h;
}

/*DLL_PUBLIC*/ bool operator>( const GSize &s1, const GSize &s2 )
{
    return s1.m_data.w *s1.m_data.h > s2.m_data.w*s2.m_data.h;
}

/*DLL_PUBLIC*/ const GSize operator+ ( const GSize & s1, const GSize & s2 )
{
    return GSize ( s1.m_data.w+s2.m_data.w, s1.m_data.h+s2.m_data.h );
}

/*DLL_PUBLIC*/ const GSize operator- ( const GSize &s1, const GSize &s2 )
{
    return GSize ( s1.m_data.w-s2.m_data.w, s1.m_data.h-s2.m_data.h );
}

/*DLL_PUBLIC*/ const GSize operator* ( const GSize &s, double c )
{
    return GSize ( gRound ( s.m_data.w*c ), gRound ( s.m_data.h*c ) );
}

/*DLL_PUBLIC*/ const GSize operator* ( double c, const GSize &s )
{
    return GSize ( gRound ( s.m_data.w*c ), gRound ( s.m_data.h*c ) );
}

GSize &GSize::operator/= ( double c )
{
    G_ASSERT ( !gIsNear0 ( c ) );
    m_data.w = gRound ( m_data.w/c );
    m_data.h = gRound ( m_data.h/c );
    return *this;
}

/*DLL_PUBLIC*/ const GSize operator/ ( const GSize &s, double c )
{
    G_ASSERT ( !gIsNear0 ( c ) );
    return GSize ( gRound ( s.m_data.w/c ), gRound ( s.m_data.h/c ) );
}

GSize GSize::expandedTo ( const GSize & otherSize ) const
{
    return GSize ( gMax ( m_data.w,otherSize.m_data.w ), gMax ( m_data.h,otherSize.m_data.h ) );
}

GSize GSize::boundedTo ( const GSize & otherSize ) const
{
    return GSize ( gMin ( m_data.w,otherSize.m_data.w ), gMin ( m_data.h,otherSize.m_data.h ) );
}

void GSize::transpose()
{
    T_OFFSET tmp = m_data.w;
    m_data.w = m_data.h;
    m_data.h = tmp;
    
}

// have a nice day ^_^
// have a nice day ^_^
