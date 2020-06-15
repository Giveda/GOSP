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

#ifndef GEVENT_H
#define GEVENT_H

#include <gRect.h>
#include <gString.h>
#include <stdint.h>
#include <gConstDefine.h>
#include <gGlobal.h>

#define SYS_EVT_PRIORITY  0

/*! @file  gEvent.h
 * @brief  GEvent 事件\n
 * 
 * @author 明心
 * @version 1.0.0
 * @date 2019-2-6
 */

/*!
 * @class GEvent
 * @brief GEvent 是所有事件的基类
 * 
 */
class GEvent
{
public:
    enum EVT_TYPE 
    {
        KEYPRESS_EVT=0,
        KEYRELEASE_EVT,
        PAINT_EVT,
        TIMER_EVT,
        BACKEND_NOTIFY_IO_EVT,
        MOUSEPRESS_EVT,
        SWIPE_EVT,
        GESTURE_SCROLL_EVT,
        TAP_EVT,
        USR_EVT_START=0x20001,
    };
    GEvent(EVT_TYPE t):m_type(t) {}
    virtual ~GEvent() {}
    EVT_TYPE  type() const {
        return m_type;
    }
    virtual bool canComposite ();
    virtual void composite ( GEvent* );
    virtual bool isEqual2 ( GEvent* evt );

protected:
    EVT_TYPE  m_type;
};

class  GKeyEvent : public GEvent
{
public:
    GKeyEvent( EVT_TYPE type, int keyCode, bool autorep=false );
    int     key() const;
    int     ascii() const;
    GString text() const;
    virtual bool canComposite ();
    virtual bool isEqual2 ( GEvent* evt );

protected:
    int k;
    uint16_t unicode;
    bool repeat;
};

class  GPaintEvent : public GEvent
{
public:
    GPaintEvent( const GRect &paintRect, bool erased = true )
        : GEvent(PAINT_EVT),
          rec(paintRect),
          erase(erased) {}
    const GRect &rect() const   {
        return rec;
    }
    bool erased() const {
        return erase;
    }
protected:
    GRect rec;
    bool erase;
};

class GTimer;
class GTimerEvent : public GEvent
{
public:
    GTimerEvent ( GTimer * t): GEvent( TIMER_EVT ), m_timer(t) {}
    GTimer *timer() {
        return m_timer;
    }
    virtual bool canComposite ();
    virtual bool isEqual2 ( GEvent* evt );

private:
    GTimer *m_timer;
};

#endif 


// nice day ^_^
