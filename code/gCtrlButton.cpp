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

#ifdef CONFIG_gCtrlButton

#ifndef GCTRLBUTTON_H
#define GCTRLBUTTON_H

#include <gMItem.h>

class GCtrlButtonPrivate;

/*! @file  gCtrlButton.h
 * @brief  GCtrlButton 按钮。
 * 
 * @author 明心
 * @version 1.0.0
 * @date 2019-2-4
 */

/*!
 * @class GCtrlButton
 * @brief 按钮
 * 
 */
class DLL_PUBLIC GCtrlButton : public GMCtrlItem
{
    SET_CLASS_NAME( GCtrlButton )
    G_DISABLE_ASSIGN ( GCtrlButton )
#ifndef CONFIG_STD_CPP11
    DEFINE_SIGNAL(T_pvrv, clicked)
#else
signals:
    ///当按钮被点击时，将发射本信号。
    GSignal<void(void)> clicked;
#endif

public:
    /**
     * @brief 构建一个默认风格的按钮，按钮的文字标题为空（未设置）
     * 
     * @param form 按钮所在的窗体
     * @param parent ...
     * @param name ...
     */
    GCtrlButton ( GCtrlForm* form, GMItem* parent=0, const char* name=0 );
    
    /**
     * @brief 构建一个默认风格的按钮，按钮的文字标题为 strCaption
     * 
     * @param strCaption 按钮的文字标题
     * @param form 按钮所在的窗体
     * @param parent ...
     * @param name ...
     */
    GCtrlButton ( const GString& strCaption, GCtrlForm* form, GMItem* parent=0, const char* name=0 );
    
    /**
     * @brief 构建一个按钮，按钮的文字标题为 strCaption，背景图片为 bg ，焦点图片为 focus 
     * 
     * @param strCaption 按钮的文字标题
     * @param bg 按钮的背景图片
     * @param focus 按钮的焦点图片
     * @param form 按钮所在的窗体
     * @param parent ...
     * @param name ...
     */
    GCtrlButton ( const GString& strCaption, const GImage& bg, const GImage& focus, GCtrlForm* form, GMItem* parent=0, const char* name=0 );
    
    virtual ~GCtrlButton();

    /**
     * @brief 设置按钮的文字标题
     * 
     * @param strCaption 按钮的文字标题
     * @return void
     */
    void setCaption ( GString strCaption );
    
    /**
     * @brief 设置按钮的背景图片
     * 
     * @param pix 背景图片
     * @return void
     */
    void setBgPixmap ( const GImage &pix ); 
    
    /**
     * @brief 设置按钮的焦点图片
     * 
     * @param pix 焦点图片
     * @return void
     */
    void setFocusPixmap ( const GImage &pix ); 
    
protected:
    virtual void paintEvent ( );
	
#if defined(CONFIG_KEY_PRESS_EVENT_ENABLED)
    virtual bool fwKeyPressEvent ( GKeyEvent* );
#endif
	
#ifdef CONFIG_MOUSE_EVENT_ENABLED
    virtual bool fwMousePressEvent ( GMouseEvent* );
#endif
	
#ifdef CONFIG_TOUCH_EVENT_ENABLED
	virtual bool fwTapEvent(GTapEvent*);
#endif
	
private:
    GCtrlButtonPrivate *btnPriv;
};

#endif

#endif  //CONFIG_gCtrlButton

