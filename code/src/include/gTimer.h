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

#ifndef GTIMER_H
#define GTIMER_H

#include <gObject.h>
#include <gConstDefine.h>
#include <gGlobal.h>

class GTimerData;

class  GTimer : public GObject
{
    SET_CLASS_NAME(GTimer)
    G_DISABLE_COPY(GTimer, GObject)
public:
    GTimer( GObject *parent=0, const char *name=0 );
    virtual ~GTimer();

    bool isActive() const;

    int   start( int msec, bool sshot = false );
    void stop();
    int   restart();
    long int id() const;

    /**
     * @brief 执行一个一次性定时操作。超时会执行指定的槽函数
     *
     * @param msec 超时，单位为毫秒
     * @param receiver 接收者
     * @param SlotFunc  接收者的槽函数（用于接收超时信号）
     * @return void
     */
    template<class Receiver>
    static void singleShot(int msec, Receiver *receiver, void ( Receiver::*SlotFunc ) () )
    {
        GTimer *t= new GTimer(NULL, "singleShotTimer");
        connect( t,  t->timeout,  receiver, SlotFunc);
        t->start(msec, true);
    }

    bool isNeedDelete();

signals:
    GSignal<void(void)>  timeout;

protected:

private:
    GTimerData *timerData;
};

#endif 


// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
