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

#ifdef CONFIG_gEvent

#ifndef GEVENT_H
#define GEVENT_H

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
class DLL_PUBLIC GEvent
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
    
    /**
     * @brief 使用指定的事件类型构造一个事件对象
     * 
     * @param type ...
     */
    GEvent(uint32_t type);
    
    virtual ~GEvent();
    
    /**
     * @brief 返回事件的类型
     * 
     * @return uint32_t
     */
    uint32_t  type() const;
    
    /**
     * @brief 获取是否需要再次分发
     * 
     * @return bool
     */
    bool dispatchAgain();
    
    /**
     * @brief 再次分发此事件
     * 
     * @return void
     */
    void setDispatchAgain();
    
    /**
     * @brief 不再分发此事件
     * 
     * @return void
     */
    void resetDispatchAgain();
    
    /**
     * @brief 是否支持合并事件
     * 
     * @return bool
     */
    virtual bool canComposite ();
    
    /**
     * @brief 合并事件
     * 
     * @param  ...
     * @return void
     */
    virtual void composite ( GEvent* );
    
    /**
     * @brief 判断本事件与 evt 是否是相同事件
     * 
     * @param evt ...
     * @return bool
     */
    virtual bool isEqual2 ( GEvent* evt );
    
protected:
    uint32_t  m_type;
    bool        m_dispatchAgain;
};

#endif // GEVENT_H

#endif  //CONFIG_gEvent
