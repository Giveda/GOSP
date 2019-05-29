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

#ifdef CONFIG_gUIEvent

#ifndef GUIEVENT_H
#define GUIEVENT_H

#include <gGlobal.h>
#include <gEvent.h>
#include <gRect.h>
#include <gRegion.h>
#include <gString.h>
#include <stdint.h>

namespace Giveda
{
    enum AlignmentFlag {
        AlignLeft = 0x0001,
//         AlignRight = 0x0002,
        AlignHCenter = 0x0004,
//         AlignJustify = 0x0008,
//         AlignAbsolute = 0x0010,
//         AlignHorizontal_Mask = AlignLeft | AlignRight | AlignHCenter | AlignJustify | AlignAbsolute,
        
//         AlignTop = 0x0020,
//         AlignBottom = 0x0040,
        AlignVCenter = 0x0080,
//         AlignVertical_Mask = AlignTop | AlignBottom | AlignVCenter,
        
        AlignCenter = AlignVCenter | AlignHCenter
    };
    enum TextFlag {
        TextSingleLine = 0x0100,
        TextDontClip = 0x0200,
        TextExpandTabs = 0x0400,
        TextShowMnemonic = 0x0800,
        TextWordWrap = 0x1000,
        TextWrapAnywhere = 0x2000,
        TextDontPrint = 0x4000,
        TextIncludeTrailingSpaces = 0x08000000,
        TextHideMnemonic = 0x8000,
        TextJustificationForced = 0x10000,
        TextForceLeftToRight = 0x20000,
        TextForceRightToLeft = 0x40000,
        TextLongestVariant = 0x80000,
        TextBypassShaping = 0x100000,
        
        SingleLine = TextSingleLine,
        DontClip = TextDontClip,
        ExpandTabs = TextExpandTabs,
        ShowPrefix = TextShowMnemonic,
        WordBreak = TextWordWrap,
        BreakAnywhere = TextWrapAnywhere,
        DontPrint = TextDontPrint,
        IncludeTrailingSpaces = TextIncludeTrailingSpaces,
        NoAccel = TextHideMnemonic
    };
};

/*! @file  gUIEvent.h
 * @brief  Giveda::AlignmentFlag 文本对齐标志\n
 * Giveda::TextFlag 文本排版标志\n
 * GMouseEvent 鼠标事件\n
 * GKeyEvent 键盘事件\n
 * GPaintEvent 绘制事件\n
 * GTapEvent  触摸点击事件\n
 * GSwipeEvent 触摸滑动事件\n
 * GGestureScrollEvent 触摸拖动事件
 * 
 * @author 明心
 * @version 1.0.0
 * @date 2019-2-6
 */

#ifdef CONFIG_MOUSE_EVENT_ENABLED
/*!
 * @class GMouseEvent
 * @brief GMouseEvent 鼠标事件
 * 
 */
class DLL_PUBLIC GMouseEvent : public GEvent
{
public:
    /**
     * @brief 构造一个鼠标事件
     * 
     * @param type 鼠标事件的具体类型
     * @param pos 点击位置坐标
     * @param btn 鼠标按钮
     */
    GMouseEvent ( GEvent::EVT_TYPE type, const GPoint & pos, Giveda::MouseButton btn);
    
    /**
     * @brief 点击的位置坐标
     * 
     * @return GPoint&
     */
    GPoint& pos();
    
    /**
     * @brief 获取是哪个鼠标按钮
     * 
     * @return Giveda::MouseButton
     */
    Giveda::MouseButton  button() const;
    
private:
    Giveda::MouseButton m_btn;
    GPoint m_pos;
};
#endif

#if defined(CONFIG_KEY_PRESS_EVENT_ENABLED) || defined(CONFIG_KEY_RELEASE_EVENT_ENABLED)
/*!
 * @class GKeyEvent
 * @brief GKeyEvent 键盘事件
 * 
 */
class  DLL_PUBLIC GKeyEvent : public GEvent
{
public:
    /**
     * @brief 构造一个键盘事件
     * 
     * @param type 键盘事件的具体类型
     * @param keyCode 键码
     * @param autorep 是否是自动重复的键盘事件
     */
    GKeyEvent( GEvent::EVT_TYPE type, Giveda::Key keyCode, bool autorep = false );
    
    /**
     * @brief 获取键码
     * 
     * @return Giveda::Key
     */
    Giveda::Key     key() const;
    
    /**
     * @brief 获取 ascii 字符
     * 
     * @return int
     */
    int     ascii() const;
    
    /**
     * @brief 获取 string 
     * 
     * @return GString
     */
    GString text() const;
    
    /**
     * @brief 是否是自动重复的键盘事件
     * 
     * @return bool
     */
    bool isAutoRepeat () const;
    
    /**
     * @brief 是否可以合并 
     * 
     * @return bool
     */
    virtual bool canComposite ();
    
    /**
     * @brief 是否与 evt 事件相同
     * 
     * @param evt ...
     * @return bool
     */
    virtual bool isEqual2 ( GEvent* evt );
    
protected:
    Giveda::Key k;
    uint16_t unicode;
    bool repeat;
};
#endif

#if !defined(CONFIG_GUI_MODE_NONE)
/*!
 * @class GPaintEvent
 * @brief GPaintEvent 绘制事件
 * 
 */
class  DLL_PUBLIC GPaintEvent : public GEvent
{
public:
#define  ALL_SCREEN   -1
    /**
     * @brief 构造一个绘制事件
     * 
     * @param paintRect 绘制区域
     * @param screen 哪个屏幕
     * @param erased 是否擦除
     */
    GPaintEvent( const GRect &paintRect, int8_t screen=ALL_SCREEN, bool erased = true );
    
    /**
     * @brief 获取绘制区域
     * 
     * @return const GRect&
     */
    const GRect &rect() const;
    
    /**
     * @brief 获取绘制区域
     * 
     * @return const GRegion&
     */
    const GRegion &region() const;
    
    /**
     * @brief 是否已经擦除
     * 
     * @return bool
     */
    bool erased() const;
    
    /**
     * @brief 获取屏幕索引
     * 
     * @return int8_t
     */
    int8_t screen();
    
    /**
     * @brief 能否合并
     * 
     * @return bool
     */
    virtual bool canComposite ();
    
    /**
     * @brief 合并事件
     * 
     * @param e ...
     * @return void
     */
    virtual void composite ( GEvent* e );
    
protected:
    GRect rec;
    GRegion reg;
    bool erase;
    int8_t screenIdx;
};
#endif

#ifdef CONFIG_TOUCH_EVENT_ENABLED
/*!
 * @class GTapEvent
 * @brief GTapEvent  触摸点击事件
 * 
 */
class DLL_PUBLIC GTapEvent : public GEvent
{
public:
    /**
     * @brief 构造一个触摸点击事件
     * 
     * @param pos 点击位置坐标
     */
    GTapEvent ( const GPoint & pos);
    
    /**
     * @brief 获取点击位置坐标
     * 
     * @return GPoint&
     */
    GPoint& pos();

private:
    GPoint m_pos;
};

/*!
 * @class GSwipeEvent
 * @brief GSwipeEvent 触摸滑动事件
 * 
 */
class DLL_PUBLIC GSwipeEvent : public GEvent
{
public:
    ///滑动方向
    enum SWIPE_DIRECTION 
    {
        ///向左滑动
        SWIPE_LEFT=0,
        
        ///向右滑动
        SWIPE_RIGHT,
        
        ///向上滑动
        SWIPE_UP,
        
        ///向下滑动
        SWIPE_DOWN
    };
    /**
     * @brief 构造一个滑动事件
     * 
     * @param d 滑动方向
     * @param startPos 起始位置坐标
     */
    GSwipeEvent ( GSwipeEvent::SWIPE_DIRECTION d, const GPoint &startPos);
    
    /**
     * @brief 获取滑动方向
     * 
     * @return GSwipeEvent::SWIPE_DIRECTION
     */
    GSwipeEvent::SWIPE_DIRECTION  direction() const;
    
    /**
     * @brief 获取滑动的起始位置坐标
     * 
     * @return GPoint&
     */
    GPoint& pos();
    
private:
    GSwipeEvent::SWIPE_DIRECTION m_direction;
    GPoint m_pos;
};

/*!
 * @class GGestureScrollEvent
 * @brief GGestureScrollEvent 触摸拖动事件
 * 
 */
class DLL_PUBLIC GGestureScrollEvent : public GEvent
{
public:
    ///拖动方向
    enum SWIPE_DIRECTION 
    {
        ///向左拖动
        SWIPE_LEFT=0,
        
        ///向右拖动
        SWIPE_RIGHT,
        
        ///向上拖动
        SWIPE_UP,
        
        ///向下拖动
        SWIPE_DOWN
    };
    /**
     * @brief 构造一个触摸拖动事件
     * 
     * @param d 拖动方向
     * @param startPos 起始位置坐标
     * @param distance 拖动的距离
     */
    GGestureScrollEvent ( GGestureScrollEvent::SWIPE_DIRECTION d, const GPoint &startPos, const T_OFFSET distance);
    
    /**
     * @brief 获取拖动方向
     * 
     * @return GGestureScrollEvent::SWIPE_DIRECTION
     */
    GGestureScrollEvent::SWIPE_DIRECTION  direction() const;
    
    /**
     * @brief 获取拖动的起始位置坐标
     * 
     * @return GPoint&
     */
    GPoint& pos();
    
    /**
     * @brief 获取拖动的距离
     * 
     * @return T_OFFSET
     */
    T_OFFSET distance() const;
    
private:
    GGestureScrollEvent::SWIPE_DIRECTION m_direction;
    GPoint m_pos;
    T_OFFSET m_distance;
};
#endif  //CONFIG_TOUCH_EVENT_ENABLED

#endif // GEVENT_H

#endif  //CONFIG_gUIEvent
