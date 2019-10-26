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

#include "gTimer.h"

#include <signal.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <gConstDefine.h>
#include <gGlobal.h>
#include <gUIEvtLoop.h>
#include <eventlist_p.h>
#include <gEvent.h>

void addTimer ( GTimer* t );

void deleteTimer ( GTimer* t );

void  timer_notify_function(union sigval  d)
{
    GTimer *t = (GTimer*)d.sival_ptr;
    putEvt( new GTimerEvent(t) );
}

class GTimerData
{
public:
    GTimerData()
        :id(NULL), needDelete(false), active(false), singleShot(false), msec(0)
    {
        its.it_value.tv_sec = 0;
        its.it_value.tv_nsec = 0;
        its.it_interval.tv_sec = its.it_value.tv_sec; 
        its.it_interval.tv_nsec = its.it_value.tv_nsec;
    }
    timer_t     id;
    bool needDelete;
    bool active;
    bool singleShot;
    int msec;
    struct itimerspec its;
};

GTimer::GTimer ( GObject* parent, const char* name ) : GObject ( parent, name )
{
    timerData = new GTimerData;
    sigevent_t  se;
    se.sigev_notify = SIGEV_THREAD;
    se.sigev_notify_function = timer_notify_function;
    se.sigev_notify_attributes = NULL;
    se.sigev_value.sival_ptr = (void *)this;
    int ret = timer_create(CLOCK_REALTIME, &se, &timerData->id);
    if(-1 == ret)
    {
        ERROR("timer_create: [%s]\n", strerror(errno) );
        exit(-1);
    }
    addTimer(this);
}

GTimer::~GTimer()
{
    deleteTimer(this);
    if(timerData->id)
    {
        timer_delete(timerData->id);
        return ;
    }
    delete timerData;
}

int GTimer::start ( int msec, bool sshot )
{
    int ret = 0;
    timerData->its.it_value.tv_sec = msec / 1000;
    timerData->its.it_value.tv_nsec = msec % 1000*1000000;
    timerData->singleShot = sshot;
    timerData->msec = msec;
    if(sshot)
    {
        timerData->its.it_interval.tv_sec = 0; 
        timerData->its.it_interval.tv_nsec = 0;
    }
    else
    {
        timerData->its.it_interval.tv_sec = timerData->its.it_value.tv_sec; 
        timerData->its.it_interval.tv_nsec = timerData->its.it_value.tv_nsec;
    }
    ret = timer_settime(timerData->id, 0, &timerData->its, NULL);
    if(-1 == ret)
    {
        WARNING("timer_settime: [%s]\n", strerror(errno) );
        return -10;
    }

    timerData->active = true;
    return 0;
}

void GTimer::stop()
{
    timerData->its.it_value.tv_sec = 0;
    timerData->its.it_value.tv_nsec = 0;
    int ret = timer_settime(timerData->id, 0, &timerData->its, NULL);
    if(-1 == ret)
    {
        WARNING("timer_settime: [%s]\n", strerror(errno) );
        return ;
    }
    timerData->active = false;
}

bool GTimer::isActive() const
{
    return timerData->active;
}

long int GTimer::id() const
{
    return reinterpret_cast<long>(timerData->id);
}

bool GTimer::isNeedDelete()
{
    return timerData->needDelete;
}

int GTimer::restart()
{
    return start(timerData->msec, timerData->singleShot);
    
}

// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
