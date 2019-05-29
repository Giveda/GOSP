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

#ifdef CONFIG_gTimer

#ifndef GTIMER_H
#define GTIMER_H

#include <gObject.h>

class GTimerData;

/*! @file  gTimer.h
 * @brief  GTimer  定时器
 * 
 * @author 明心
 * @version 1.0.0
 * @date 2019-2-6
 */

/*!
 * @class GTimer
 * @brief GTimer 定时器
 * 
 */
class  DLL_PUBLIC GTimer : public GObject
{
    friend class GTimerEvent;
    G_DISABLE_COPY(GTimer, GObject)
#ifndef CONFIG_STD_CPP11
    DEFINE_SIGNAL(T_pvrv, timeout)
#else
signals:
    ///当定时器超时，该信号被发射。
    GSignal<void(void)>  timeout;
#endif
public:
    /**
     * @brief 构造一个定时器
     * 
     * @param parent ...
     * @param name ...
     * @param sshot 系统保留参数，开发者不可使用，否则将给你带来莫名其妙的问题
     */
    GTimer( GObject *parent=0, const char *name=0, bool sshot=false );
    
    virtual ~GTimer();

    /**
     * @brief 是否正在运行
     * 
     * @return bool
     */
    bool isActive() const;

    /**
     * @brief 启动定时器
     * 
     * @param msec 超时，单位为毫秒
     * @param sshot 
     * @return int 是否single shot（超时一次就停止的定时器）
     */
    int   start( int msec, bool sshot = false );
    
    /**
     * @brief 停止定时器
     * 
     * @return void
     */
    void stop();
    
    /**
     * @brief 重新启动定时器
     * 
     * @return int
     */
    int   restart();
    
#ifndef CONFIG_STD_CPP11
    static void singleShot(int msec, GObject *receiver, T_pvrv member );
#else
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
        GTimer *t= new GTimer(NULL, "singleShotTimer", true);
        connect( t,  t->timeout,  receiver, SlotFunc);
        t->start(msec, true);
    }
#endif

public:
    virtual bool event(GEvent*);

private:
    /**
     * @brief 定时器ID
     * 
     * @return long int
     */
    DLL_LOCAL long int id() const;
    
private:
    GTimerData *timerData;
};

#endif // GTIMER_H

#endif  //CONFIG_gTimer
