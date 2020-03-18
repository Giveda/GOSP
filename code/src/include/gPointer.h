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


#ifndef GGUARDEDPTR_H
#define GGUARDEDPTR_H

#include <gObject.h>
#include <gConstDefine.h>
#include <gGlobal.h>

class  GPointerData
{
public:
    GPointerData ( GObject* );
    virtual ~GPointerData();

    GObject* object() const
    {
        return m_pObject;
    }
private:
    GObject* m_pObject;
};

template <class T> class  GPointer
{
public:
    GPointer()
    {
        m_pData = new GPointerData ( 0 );
    }
    GPointer ( T* o )
    {
        m_pData = new GPointerData ( o );
    }
    GPointer ( const GPointer<T> &p )
    {
        m_pData = p.priv;
    }
    ~GPointer()
    {
    }

    GPointer<T> &operator= ( const GPointer<T> &p )
    {
        if ( m_pData != p.priv )
        {
            m_pData = p.priv;
        }
        return *this;
    }

    GPointer<T> &operator= ( T* o )
    {
        m_pData = new GPointerData ( o );

        return *this;
    }

    bool operator== ( const GPointer<T> &p ) const
    {
        return m_pData->object() == p.priv->object();
    }

    bool operator!= ( const GPointer<T>& p ) const
    {
        return ! ( *this == p );
    }

    bool isNull() const
    {
        return !m_pData->object();
    }

    T* operator->() const
    {
        return ( T* ) m_pData->object();
    }

    T& operator*() const
    {
        return * ( ( T* ) m_pData->object() );
    }

    operator T*() const
    {
        return ( T* ) m_pData->object();
    }


private:
    GPointerData* m_pData;
};

#endif


// reborn, a nice day ^_^
