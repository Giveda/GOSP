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

#include "gRect.h"
#include "gGlobal.h"
#include <gConstDefine.h>
#include <gGlobal.h>

#include <math.h>
#include <netinet/in.h>

GRect::GRect()
{
    m_rectData.left = m_rectData.top = 0;
    m_rectData.right = m_rectData.bottom = -1;
}

GRect GRect::operator& ( const GRect &r ) const
{
    if ( isNull() || r.isNull() )
        return GRect();

    T_OFFSET l1 = m_rectData.left;
    T_OFFSET r1 = m_rectData.left;
    if ( m_rectData.right - m_rectData.left + 1 < 0 )
        l1 = m_rectData.right;
    else
        r1 = m_rectData.right;

    T_OFFSET l2 = r.m_rectData.left;
    T_OFFSET r2 = r.m_rectData.left;
    if ( r.m_rectData.right - r.m_rectData.left + 1 < 0 )
        l2 = r.m_rectData.right;
    else
        r2 = r.m_rectData.right;

    if ( l1 > r2 || l2 > r1 )
        return GRect();

    T_OFFSET t1 = m_rectData.top;
    T_OFFSET b1 = m_rectData.top;
    if ( m_rectData.bottom - m_rectData.top + 1 < 0 )
        t1 = m_rectData.bottom;
    else
        b1 = m_rectData.bottom;

    T_OFFSET t2 = r.m_rectData.top;
    T_OFFSET b2 = r.m_rectData.top;
    if ( r.m_rectData.bottom - r.m_rectData.top + 1 < 0 )
        t2 = r.m_rectData.bottom;
    else
        b2 = r.m_rectData.bottom;

    if ( t1 > b2 || t2 > b1 )
        return GRect();

    GRect tmp;
    tmp.m_rectData.left = gMax ( l1, l2 );
    tmp.m_rectData.right = gMin ( r1, r2 );
    tmp.m_rectData.top = gMax ( t1, t2 );
    tmp.m_rectData.bottom = gMin ( b1, b2 );
    return tmp;
}

GRect::GRect ( T_OFFSET aleft, T_OFFSET atop, T_OFFSET awidth, T_OFFSET aheight )
{
    m_rectData.left = aleft;
    m_rectData.top = atop;
    m_rectData.right = ( aleft + awidth - 1 );
    m_rectData.bottom = ( atop + aheight - 1 );
}

GRect::GRect ( const GPoint &atopLeft, const GSize &asize )
{
    m_rectData.left = atopLeft.x();
    m_rectData.top = atopLeft.y();
    m_rectData.right = ( m_rectData.left+asize.width() - 1 );
    m_rectData.bottom = ( m_rectData.top+asize.height() - 1 );
}

bool GRect::isNull() const
{
    return m_rectData.right == m_rectData.left - 1 && m_rectData.bottom == m_rectData.top - 1;
}

bool GRect::isEmpty() const
{
    return m_rectData.left > m_rectData.right || m_rectData.top > m_rectData.bottom;
}

bool GRect::isValid() const
{
    return m_rectData.left <= m_rectData.right && m_rectData.top <= m_rectData.bottom;
}

T_OFFSET GRect::left() const
{
    return m_rectData.left;
}

T_OFFSET GRect::top() const
{
    return m_rectData.top;
}

T_OFFSET GRect::right() const
{
    return m_rectData.right;
}

T_OFFSET GRect::bottom() const
{
    return m_rectData.bottom;
}

T_OFFSET GRect::x() const
{
    return m_rectData.left;
}

T_OFFSET GRect::y() const
{
    return m_rectData.top;
}

GPoint GRect::topLeft() const
{
    return GPoint ( m_rectData.left, m_rectData.top );
}

GPoint GRect::bottomRight() const
{
    return GPoint ( m_rectData.right, m_rectData.bottom );
}

GPoint GRect::topRight() const
{
    return GPoint ( m_rectData.right, m_rectData.top );
}

GPoint GRect::bottomLeft() const
{
    return GPoint ( m_rectData.left, m_rectData.bottom );
}

GPoint GRect::center() const
{
    return GPoint ( ( m_rectData.left+m_rectData.right ) /2, ( m_rectData.top+m_rectData.bottom ) /2 );
}

T_OFFSET GRect::width() const
{
    return  m_rectData.right - m_rectData.left + 1;
}

T_OFFSET GRect::height() const
{
    return  m_rectData.bottom - m_rectData.top + 1;
}

GSize GRect::size() const
{
    return GSize ( width(), height() );
}

void GRect::translate ( T_OFFSET dx, T_OFFSET dy )
{
    m_rectData.left += dx;
    m_rectData.top += dy;
    m_rectData.right += dx;
    m_rectData.bottom += dy;
}

void GRect::translate ( const GPoint &p )
{
    m_rectData.left += p.x();
    m_rectData.top += p.y();
    m_rectData.right += p.x();
    m_rectData.bottom += p.y();
}

void GRect::moveLeft ( T_OFFSET pos )
{
    m_rectData.right += ( pos - m_rectData.left );
    m_rectData.left = pos;
}

void GRect::moveTop ( T_OFFSET pos )
{
    m_rectData.bottom += ( pos - m_rectData.top );
    m_rectData.top = pos;
}

void GRect::moveRight ( T_OFFSET pos )
{
    m_rectData.left += ( pos - m_rectData.right );
    m_rectData.right = pos;
}

void GRect::moveBottom ( T_OFFSET pos )
{
    m_rectData.top += ( pos - m_rectData.bottom );
    m_rectData.bottom = pos;
}

void GRect::moveTopLeft ( const GPoint &p )
{
    moveLeft ( p.x() );
    moveTop ( p.y() );
}

void GRect::setWidth ( T_OFFSET w )
{
    m_rectData.right = ( m_rectData.left + w - 1 );
}

void GRect::setHeight ( T_OFFSET h )
{
    m_rectData.bottom = ( m_rectData.top + h - 1 );
}

void GRect::setSize ( const GSize &s )
{
    m_rectData.right = ( s.width()  + m_rectData.left - 1 );
    m_rectData.bottom = ( s.height() + m_rectData.top - 1 );
}

GRect GRect::intersect ( const GRect &r ) const
{
    return *this & r;
}

void GRect::moveBy ( T_OFFSET dx, T_OFFSET dy )
{
    translate ( dx, dy );
}

void GRect::moveBy ( const GPoint& p )
{
    translate ( p );
}

void GRect::setRect(T_OFFSET ax, T_OFFSET ay, T_OFFSET aw, T_OFFSET ah)
{
    m_rectData.left = ax;
    m_rectData.top = ay;
    m_rectData.right = (ax + aw - 1);
    m_rectData.bottom = (ay + ah - 1);
}

bool GRect::contains(const GRect &r, bool proper) const
{
    if (isNull() || r.isNull())
        return false;
    
    T_OFFSET l1 = m_rectData.left;
    T_OFFSET r1 = m_rectData.left;
    if (m_rectData.right - m_rectData.left + 1 < 0)
        l1 = m_rectData.right;
    else
        r1 = m_rectData.right;
    
    T_OFFSET l2 = r.m_rectData.left;
    T_OFFSET r2 = r.m_rectData.left;
    if (r.m_rectData.right - r.m_rectData.left + 1 < 0)
        l2 = r.m_rectData.right;
    else
        r2 = r.m_rectData.right;
    
    if (proper) {
        if (l2 <= l1 || r2 >= r1)
            return false;
    } else {
        if (l2 < l1 || r2 > r1)
            return false;
    }
    
    T_OFFSET t1 = m_rectData.top;
    T_OFFSET b1 = m_rectData.top;
    if (m_rectData.bottom - m_rectData.top + 1 < 0)
        t1 = m_rectData.bottom;
    else
        b1 = m_rectData.bottom;
    
    T_OFFSET t2 = r.m_rectData.top;
    T_OFFSET b2 = r.m_rectData.top;
    if (r.m_rectData.bottom - r.m_rectData.top + 1 < 0)
        t2 = r.m_rectData.bottom;
    else
        b2 = r.m_rectData.bottom;
    
    if (proper) {
        if (t2 <= t1 || b2 >= b1)
            return false;
    } else {
        if (t2 < t1 || b2 > b1)
            return false;
    }
    
    return true;
}

bool GRect::intersects(const GRect &r) const
{
    if (isNull() || r.isNull())
        return false;
    
    T_OFFSET l1 = m_rectData.left;
    T_OFFSET r1 = m_rectData.left;
    if (m_rectData.right - m_rectData.left + 1 < 0)
        l1 = m_rectData.right;
    else
        r1 = m_rectData.right;
    
    T_OFFSET l2 = r.m_rectData.left;
    T_OFFSET r2 = r.m_rectData.left;
    if (r.m_rectData.right - r.m_rectData.left + 1 < 0)
        l2 = r.m_rectData.right;
    else
        r2 = r.m_rectData.right;
    
    if (l1 > r2 || l2 > r1)
        return false;
    
    T_OFFSET t1 = m_rectData.top;
    T_OFFSET b1 = m_rectData.top;
    if (m_rectData.bottom - m_rectData.top + 1 < 0)
        t1 = m_rectData.bottom;
    else
        b1 = m_rectData.bottom;
    
    T_OFFSET t2 = r.m_rectData.top;
    T_OFFSET b2 = r.m_rectData.top;
    if (r.m_rectData.bottom - r.m_rectData.top + 1 < 0)
        t2 = r.m_rectData.bottom;
    else
        b2 = r.m_rectData.bottom;
    
    if (t1 > b2 || t2 > b1)
        return false;
    
    return true;
}

bool GRect::contains(const GPoint &p, bool proper) const
{
    T_OFFSET l, r;
    if (m_rectData.right < m_rectData.left - 1) {
        l = m_rectData.right;
        r = m_rectData.left;
    } else {
        l = m_rectData.left;
        r = m_rectData.right;
    }
    if (proper) {
        if (p.x() <= l || p.x() >= r)
            return false;
    } else {
        if (p.x() < l || p.x() > r)
            return false;
    }
    T_OFFSET t, b;
    if (m_rectData.bottom < m_rectData.top - 1) {
        t = m_rectData.bottom;
        b = m_rectData.top;
    } else {
        t = m_rectData.top;
        b = m_rectData.bottom;
    }
    if (proper) {
        if (p.y() <= t || p.y() >= b)
            return false;
    } else {
        if (p.y() < t || p.y() > b)
            return false;
    }
    return true;
}

void GRect::setLeft ( T_OFFSET pos )
{
    m_rectData.left = pos;
}

void GRect::setTop ( T_OFFSET pos )
{
    m_rectData.top = pos;
}

void GRect::setRight ( T_OFFSET pos )
{
    m_rectData.right = pos;
}

void GRect::setBottom ( T_OFFSET pos )
{
    m_rectData.bottom = pos;
}

const T_RECT GRect::toT_RECT() const
{
    T_RECT nr;
    nr.left = htons(m_rectData.left);
    nr.top = htons(m_rectData.top);
    nr.right = htons(m_rectData.right);
    nr.bottom = htons(m_rectData.bottom);
    return nr;
}

/*DLL_PUBLIC*/ bool operator==(const GRect &r1, const GRect &r2)
{
    return r1.m_rectData.left == r2.m_rectData.left
            && r1.m_rectData.right == r2.m_rectData.right
            && r1.m_rectData.top == r2.m_rectData.top
            && r1.m_rectData.bottom == r2.m_rectData.bottom;
}

GRect::GRect(const T_RECT& tr) 
{
    m_rectData.left = ntohs( tr.left );
    m_rectData.right = ntohs( tr.right );
    m_rectData.top = ntohs( tr.top );
    m_rectData.bottom = ntohs( tr.bottom );
}

// have a nice day
// have a nice day
