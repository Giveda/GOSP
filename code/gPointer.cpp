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

#ifdef CONFIG_gPointer

#ifndef GPOINTER_H
#define GPOINTER_H

#include <gObject.h>
#include <gSharedData.h>

class  DLL_PUBLIC GPointerData : public  GObject, public GSharedData
{
public:
    GPointerData ( GObject* );
    virtual ~GPointerData();

    GObject* object() const;
#ifndef CONFIG_STD_CPP11
private:
    DLL_LOCAL static void slotDestroyed(GObject* r);
#else
private slots:
    DLL_LOCAL void slotDestroyed();
#endif
private:
    GObject* m_pObject;
};

/*! @file  gPointer.h
 * @brief  GPointer 安全指针
 * 
 * @author 明心
 * @version 1.0.0
 * @date 2019-2-6
 */

/*!
 * @class GPointer
 * @brief GPointer 的行为非常类似一个普通指针，除了当指针所指向的对象被析构后 GPointer 对象被自动设置为 NULL。举例如下：\n
  \code
      GPointer<GCtrlText> label = new GCtrlText();
      label->setText("xxx, useful");

      delete label; //这里模拟有人在其它地方把label delete掉了

      //此时，label已经被自动设置为 NULL 了
      if ( label )
        label->show();//这里将不会被执行
      else 
        ZL_DEBUG("The label has been destroyed");//这条出错信息将会被打印出来
  \endcode \n
 * 
 */
template <class T> class  DLL_PUBLIC GPointer
{
public:
    GPointer()
    {
        priv = new GPointerData ( 0 );
    }
    GPointer ( T* o )
    {
        priv = new GPointerData ( o );
    }
    GPointer ( const GPointer<T> &p )
    {
        priv = p.priv;
        privRef();
    }
    virtual ~GPointer()
    {
        privDeref();
    }

    GPointer<T> &operator= ( const GPointer<T> &p )
    {
        if ( priv != p.priv )
        {
            privDeref();
            priv = p.priv;
            privRef();
        }
        return *this;
    }

    GPointer<T> &operator= ( T* o )
    {
        privDeref();
        priv = new GPointerData ( o );
        
        return *this;
    }

    bool operator== ( const GPointer<T> &p ) const
    {
        return priv->object() == p.priv->object();
    }

//    bool operator== ( const T* p ) const
//    {
//        return priv->object() == (GObject*)p;
//    }

    bool operator!= ( const GPointer<T>& p ) const
    {
        return ! ( *this == p );
    }

    bool isNull() const
    {
        return !priv->object();
    }

    T* operator->() const
    {
        return ( T* ) priv->object();
    }

    T& operator*() const
    {
        return * ( ( T* ) priv->object() );
    }

    operator T*() const
    {
        return ( T* ) priv->object();
    }


private:
    DLL_LOCAL void privRef()
    {
        priv->ref();
    }
    DLL_LOCAL void privDeref()
    {
        if ( priv->deref() )
            delete priv;
    }
    GPointerData* priv;
};

#endif

#endif  //CONFIG_gPointer

