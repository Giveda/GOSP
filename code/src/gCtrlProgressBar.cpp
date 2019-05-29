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

#ifdef CONFIG_gCtrlProgressBar

#ifndef GCTRLPROGRESSBAR_H
#define GCTRLPROGRESSBAR_H

#include <gMItem.h>

class GMProgressBarPrivate;

/*! @file  gCtrlProgressBar.h
 * @brief  GCtrlProgressBar 进度条
 * 
 * @author 明心
 * @version 1.0.0
 * @date 2019-2-4
 */

/*!
 * @class GMProgressBar
 * @brief 进度条 GMProgressBar 没有主动把自己加入 GCtrlForm 。 GMProgressBar 主要用于拼装自定义的UI控件。\n
 * 如果你并不需要拼装自定义的UI控件，请直接使用 GCtrlProgressBar 。
 * 
 */
class DLL_PUBLIC GMProgressBar: public GMCtrlItem
{
    G_DISABLE_ASSIGN ( GMProgressBar )
#ifdef CONFIG_STD_ANSI
    DEFINE_SIGNAL(T_pirv, sigProgressChanged)
#else
signals:
    ///当进度条的进度发生了变化时，该信号被立即发射。参数是当前的进度。
    GSignal<void(int)> sigProgressChanged;
#endif
    
public:
    /**
     * @brief 构造一个进度条；totalSteps等于100
     * 
     * @param form ...
     * @param parent ...
     * @param name ...
     */
    GMProgressBar ( GCtrlForm* form, GMItem* parent=0, const char* name=0 );
    
    /**
     * @brief 使用开发者指定的totalSteps去构造一个进度条
     * 
     * @param totalSteps 进度条总共多少步
     * @param form ...
     * @param parent ...
     * @param name ...
     */
    GMProgressBar ( int totalSteps, GCtrlForm* form, GMItem* parent=0, const char* name=0 );
    
    virtual ~GMProgressBar();

    /**
     * @brief 返回进度的总共有多少步数
     * 
     * @return int
     */
    int totalSteps () const;
    
    /**
     * @brief 返回当前的进度值
     * 
     * @return int
     */
    int progress () const;
    
    /**
     * @brief 返回进度条的步长
     * 
     * @return int
     */
    int stepLength () const;
    
    virtual void paintEvent (  );

    /**
     * @brief 设置进度的颜色
     * 
     * @param c ...
     * @return void
     */
    void setProgressColor ( GColor &c );
    
    /**
     * @brief 设置进度条背景的颜色
     * 
     * @param c ...
     * @return void
     */
    void setProgressBgColor ( GColor &c );

    /**
     * @brief 使用图片作为进度条背景
     * 
     * @param p ...
     * @return void
     */
    void setProgressBgImage ( const GImage& p );
    
    /**
     * @brief 使用图片来显示进度条的进度
     * 
     * @param p ...
     * @return void
     */
    void setProgressImage ( const GImage& p );
    
    /**
     * @brief 使用图片作为进度游标
     * 
     * @param p ...
     * @return void
     */
    void setProgressCursorImage ( const GImage& p );
    
    /**
     * @brief 设置进度、和进度背景的高度
     * 
     * @param h ...
     * @return void
     */
    void setProgressBgHeight ( const int h );

public slots:
    /**
     * @brief 设置进度
     * 
     * @param progress ...
     * @return void
     */
    void slotSetProgress ( int progress );
    
    /**
     * @brief 设置总步数
     * 
     * @param totalSteps ...
     * @return void
     */
    void slotSetTotalSteps ( int totalSteps );
    
    /**
     * @brief 设置单步的步长
     * 
     * @param nStepLen ...
     * @return void
     */
    void slotSetStepLength ( int nStepLen );

protected:
#ifdef CONFIG_MOUSE_EVENT_ENABLED
    virtual bool fwMousePressEvent ( GMouseEvent* e);
#endif
#ifdef CONFIG_TOUCH_EVENT_ENABLED
    virtual bool fwTapEvent(GTapEvent*);
    virtual bool fwGestureScrollEvent ( GGestureScrollEvent* e);
#endif
    
private:
    GMProgressBarPrivate  *pbPriv;
};

/*!
 * @class GCtrlProgressBar
 * @brief 进度条 GCtrlProgressBar 会主动把自己加入 GCtrlForm 。\n
 * 如果你需要拼装自定义的UI控件，请使用 GMProgressBar 。\n
 * 通常情况下，你并不需要拼装自定义的UI控件，所以应当直接使用 GCtrlProgressBar 。
 * 
 */
class DLL_PUBLIC GCtrlProgressBar: public GMProgressBar
{
public:
    /**
     * @brief 构造一个进度条；totalSteps等于100
     * 
     * @param form ...
     * @param parent ...
     * @param name ...
     */
    GCtrlProgressBar ( GCtrlForm* form, GMItem* parent=0, const char* name=0 );
    
    /**
     * @brief 使用开发者指定的totalSteps去构造一个进度条
     * 
     * @param totalSteps 进度条总共多少步
     * @param form ...
     * @param parent ...
     * @param name ...
     */
    GCtrlProgressBar ( int totalSteps, GCtrlForm* form, GMItem* parent=0, const char* name=0 );
};

#endif

#endif  //CONFIG_gCtrlProgressBar
