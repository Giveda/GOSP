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

#ifndef GUIEVTLOOP_H
#define GUIEVTLOOP_H

#include <gEvent.h>
#include <gObject.h>
#ifdef CONFIG_QT_SIMULATOR
#include <pthread.h>
#endif
#include <gConstDefine.h>
#include <gGlobal.h>

class GUIEvtLoop : public GObject
{
public:
    GUIEvtLoop( int argc, char** argv);
    virtual ~GUIEvtLoop();
    static int exec();
    static int enterLoop();
    static void exitLoop();
    static void quit();
    virtual void keyPressEvent ( GKeyEvent* ) {}
    const string libPath()
    {
        return m_strLibDir;
    }
    
    /**
     * @brief 将一个事件加入事件循环。如果你不需要指定接收者和优先级，那么你调用这个函数会比较方便。
     * 
     * @param event 事件
     * @param receiver 如果为空则代表事件由系统处理，如果不为空，则事件会直接分发给接收者进行处理。
     * @param priority 事件处理的优先级；值越高说明事件越紧急、需要优先处理。
     * @return int 0代表成功，非0代表失败
     */
    static int postEvent(GEvent *event, int priority=SYS_EVT_PRIORITY, GObject *receiver=NULL);
    
    /**
     * @brief 将一个事件加入事件循环。如果你不需要指定优先级，那么你调用这个函数会比较方便。
     * 
     * @param event 事件
     * @param receiver 如果为空则代表事件由系统处理，如果不为空，则事件会直接分发给接收者进行处理。
     * @param priority 事件处理的优先级；值越高说明事件越紧急、需要优先处理。
     * @return int 0代表成功，非0代表失败
     */
    static int postEvent(GEvent *event, GObject *receiver, int priority=SYS_EVT_PRIORITY);

#if defined(CONFIG_V_SCREEN_ENABLED) || !defined(CONFIG_R_SCREEN_DISABLED)
    /**
     * @brief 设置GUI窗口系统的宽高，以像素为单位。建议用户将GUI窗口系统的宽高设置为自己屏幕的分辨率。
     * 
     * @param w 宽
     * @param h 高
     * @return void
     */
    void setViewSize(int w, int h);
#endif
    
private:
    string m_strLibDir;
#ifdef CONFIG_QT_SIMULATOR
    pthread_t qtThreadID;
#endif
};

extern GUIEvtLoop *uiEvtLp;

#include <gPaintDevice.h>
extern GPaintDevice    *g_screen;

#endif  


// nice day ^_^
// for fun ^_^
