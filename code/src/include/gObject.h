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

#ifndef GOBJECT_CPP11_H
#define GOBJECT_CPP11_H

#include <gEvent.h>
#include <string.h>
#include <string>

#include <cxxabi.h>
#include <stdlib.h>
#include <list>
#include <gConstDefine.h>
#include <gGlobal.h>

using namespace std;

class GObjectFles;
class GObject;

#define slots
#define signals public

enum E_SLOT_TYPE
{
    C_SLOT_TYPE,
    CPP_SLOT_TYPE,
};

template<typename F>
class GSlotAPI;
template<typename Ret, typename ...Args>
class  GSlotAPI<Ret(Args...)>
{
public:
    E_SLOT_TYPE type() const
    {
        return m_type;
    }

public:
    explicit GSlotAPI(void* slot, GObject* receiver=NULL, E_SLOT_TYPE t=C_SLOT_TYPE )
        :m_receiver(receiver),
         m_slot(slot),
         m_type(t)
    {
    }
    virtual ~GSlotAPI() {}
    virtual void operator() ( Args&... args ) = 0;

private:
    GSlotAPI &operator= ( const GSlotAPI & ) = delete;

public:
    bool operator==(const GSlotAPI& other)
    {
        return other.m_type == m_type
               && other.m_slot == m_slot
               && other.m_receiver == m_receiver ;
    }

public:
    GObject* m_receiver;
protected:
    void* m_slot;

private:
    E_SLOT_TYPE m_type;
};

template<typename F>
class GSlotC;
template<typename Ret, typename ...Args>
/**
 * @brief C函数风格的槽
 * 
 */
class GSlotC<Ret(Args...)> : public GSlotAPI<Ret(Args...)>
{
private:
    typedef void ( *SlotFuncType ) ( Args... );

public:
    GSlotC ( SlotFuncType slot )
    : GSlotAPI<Ret(Args...)>( (void*)slot )
    {
    }
    virtual ~GSlotC() {}

public:
    void operator() ( Args&... args )
    {
        ( (SlotFuncType)GSlotAPI<Ret(Args...)>::m_slot) ( args... );
    }
};

template<typename Receiver, typename F>
class GSlotCpp;
template<typename Receiver, typename Ret, typename ...Args>
/**
 * @brief C++成员函数风格的槽
 * 
 */
class GSlotCpp<Receiver, Ret(Args...)> : public GSlotAPI<Ret(Args...)>
{
public:
    typedef void ( Receiver::*SlotFuncType ) ( Args... );

    GSlotCpp ( Receiver* r, SlotFuncType slot )
        : GSlotAPI<Ret(Args...)>( (void*)slot, (GObject*)r, CPP_SLOT_TYPE),
          m_class_slot(slot)
    {
    }

    virtual ~GSlotCpp() {}

public:
    void operator() ( Args&... args )
    {
        ( ( (Receiver*)GSlotAPI<Ret(Args...)>::m_receiver)->*m_class_slot ) ( args... );
    }

private:
    SlotFuncType m_class_slot;
};

/**
 * @class GSignal
 * @brief  GSignal 类用来定义信号，所述信号的函数类型为Ret (*)(Args...)。\n
 * 比如：GSignal<void(int)> intSig;
 * 比如：GSignal<void(int, float)> ifSig;
 *
 */
template<typename F>
class GSignal;
template<typename Ret, typename... Args>
class GSignal<Ret(Args...)>
{
public:
    typedef list<GSlotAPI<Ret(Args...)>* > SlotLstType;

    /**
     * @brief 发射信号
     *
     * @param args  参数列表
     * @return void
     */
    void emit( Args... args )
    {
        for ( auto it = _slotLst.begin(); it != _slotLst.end(); ++it )
        {
            ( * ( *it ) ) ( args... );
        }
    }

public:
    SlotLstType  _slotLst;
};


class  GSlot
{
public:
    E_SLOT_TYPE type() const
    {
        return m_type;
    }

public:
    explicit GSlot(void* slot, GObject* receiver=NULL, E_SLOT_TYPE t=C_SLOT_TYPE )
        :m_receiver(receiver),
         m_slot(slot),
         m_type(t)
    {
    }
    virtual ~GSlot() {}
    void operator() ( const GSlot& );

private:
    GSlot &operator= ( const GSlot & ) = delete;

public:
    bool operator==(const GSlot& other)
    {
        return other.m_type == m_type
               && other.m_slot == m_slot
               && other.m_receiver == m_receiver ;
    }

public:
    GObject* m_receiver;
protected:
    void* m_slot;

private:
    E_SLOT_TYPE m_type;
};

#define SIGNAL_TYPE(SlotFuncType)  list<GSlot*>
#define SIGNAL_POINTER(SlotFuncType)  list<GSlot*>*
#define SIGNAL_TYPE_ITERATOR(SlotFuncType)  list<GSlot*>::iterator

#define SET_CLASS_NAME(any_type) \
public: \
    virtual const char *className() const \
    { \
    static string s_name; \
    char* name = abi::__cxa_demangle(typeid(any_type).name(), NULL, NULL, NULL); \
    s_name = name; \
    return s_name.c_str(); \
    }

/*! @file  gObject_cpp11.h
 * @brief  使用c++11实现的 GObject
 *
 * @author 明心
 * @version 1.0.0
 * @date 2019-2-6
 */

/*!
 * @class GObject
 * @brief GObject 几乎是这个世界中所有元素的基类和祖先。
 *
 */
class GObject
{
    SET_CLASS_NAME(GObject)
signals:
    GSignal<void(void)> sigDestroyed;

private:
    GObjectFles *m_priv;

public:
    /**
     * @brief 构造一个对象
     *
     * @param parent ...
     * @param name ...
     */
    explicit GObject ( GObject *parent=NULL,  const char *name=NULL );

    GObject ( const GObject & src );
    GObject & operator= ( const GObject & src );
    virtual ~GObject();

    /**
     * @brief 将信号和槽建立连接。\n
     * Receiver代表接收者的类型
     * Args是槽函数/信号的参数列表。
     *
     * @param sender 指向发射者的指针
     * @param signal 指向信号的引用。
     * @param receiver 指向接收者的指针
     * @param SlotFunc 指向槽函数的指针
     *
     * @return 0代表成功；非0代表失败
     */
    template<class Receiver, typename ...Args>
    static int  connect ( GObject* sender, GSignal<void(Args...)>& signal, Receiver* receiver, void ( Receiver::*SlotFunc ) ( Args... ) );

    /**
     * @brief 将信号和槽断开连接。\n
     * Receiver代表接收者的类型
     * Args是槽函数/信号的参数列表。
     *
     * @param sender 指向发射者的指针
     * @param signal 指向信号的引用。
     * @param receiver 指向接收者的指针
     * @param SlotFunc 指向槽函数的指针
     *
     * @return 0代表成功；非0代表失败
     */
    template<class Receiver, typename ...Args>
    static int  disconnect ( GObject* sender, GSignal<void(Args...)>& signal, Receiver* receiver, void ( Receiver::*SlotFunc ) ( Args... ) );

    /**
     * @brief 获取对象名称
     *
     * @return const char*
     */
    const char *name() const;

    /**
     * @brief 获取对象的parent
     *
     * @return GObject*
     */
    GObject *parent() const;

    /**
     * @brief 开发者可以在子类中覆盖此函数，处理自己需要的事件
     *
     * @param  ...
     * @return bool
     */
    virtual bool event(GEvent*);

    /**
     * @brief 获取对象所在的线程ID。
     *
     * @return pthread_t 线程ID
     */
    pthread_t  tid();

private:
    static int  privConnect(GObject* sender, SIGNAL_POINTER(void*) signal, GObject* receiver, void* slot);
    static int  privDisconnect(GObject* sender, SIGNAL_POINTER(void*) signal, GObject* receiver, void* slot);
    void saveSenderPair(GObject* sender, SIGNAL_POINTER(void*) signal);
    void deleteSenderPair(GObject* sender, SIGNAL_POINTER(void*) signal);
    void destructAsReceiver();
    void destructAsSender();
    void saveReceiver ( GObject* receiver );
    void deleteReceiver ( GObject* receiver );
};

template<class Receiver, typename ...Args>
int  GObject::connect ( GObject* sender, GSignal<void(Args...)>& signal, Receiver* receiver, void ( Receiver::*SlotFunc ) ( Args... ) )
{
    GSlotCpp<Receiver, void(Args...)> *vslot = new GSlotCpp<Receiver, void(Args...)>(receiver, SlotFunc);
    int ret = privConnect(sender, reinterpret_cast<SIGNAL_POINTER(void*)>(&(signal._slotLst)), (GObject*)receiver, (void*)vslot);
    return ret;
}

template<class Receiver, typename ...Args>
int  GObject::disconnect ( GObject* sender, GSignal<void(Args...)>& signal, Receiver* receiver, void ( Receiver::*SlotFunc ) ( Args... ) )
{
    int ret = privDisconnect(sender, reinterpret_cast<SIGNAL_POINTER(void*)>(&(signal._slotLst)), (GObject*)receiver, (void*)SlotFunc);
    return ret;
}

#endif 


// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
