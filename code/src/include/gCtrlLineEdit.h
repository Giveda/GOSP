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

#ifdef CONFIG_gCtrlLineEdit

#ifndef GCTRLLINEEDIT_H
#define GCTRLLINEEDIT_H

#include <gMItem.h>

class GCtrlLineEditPrivate;

/*! @file  gCtrlLineEdit.h
 * @brief  GCtrlLineEdit 输入框、input box。
 * 
 * @author 明心
 * @version 1.0.0
 * @date 2019-2-4
 */

/*!
 * @class GCtrlLineEdit
 * @brief 输入框、input box
 * 
 */
class DLL_PUBLIC GCtrlLineEdit : public GMCtrlItem
{
    SET_CLASS_NAME( GCtrlLineEdit )
    G_DISABLE_ASSIGN ( GCtrlLineEdit )
public:
    ///此枚举用于索引输入框的显示模式：密文、或者明文
    enum	EchoMode 
    { 
        ///明文显示
        Normal, 
        
        ///密文显示
        Password 
    };
    
    /**
     * @brief 构造一个输入框
     * 
     * @param form ...
     * @param parent ...
     * @param name ...
     */
    GCtrlLineEdit ( GCtrlForm* form, GMItem* parent=0, const char* name=0 );
    
    virtual ~GCtrlLineEdit();
    
    /**
     * @brief 设置显示模式（密文、或者明文）
     * 
     * @param mode ...
     * @return void
     */
    void setEchoMode ( EchoMode mode );
    
    /**
     * @brief 获取当前的显示模式
     * 
     * @return GCtrlLineEdit::EchoMode
     */
    EchoMode 	echoMode() const;

    /**
     * @brief 设置文本的字体和字号
     * 
     * @param font ...
     * @return void
     */
    void setFont ( const GFont& font );

    /**
     * @brief 设置文本的颜色
     * 
     * @param color ...
     * @return void
     */
    void setColor ( const GColor& color );

    /**
     * @brief 设置文本内容
     * 
     * @param str ...
     * @return void
     */
    void setText ( const GString& str );

    /**
     * @brief 获取已经输入的文本内容
     * 
     * @return GString
     */
    GString text();

    /**
     * @brief 清空输入框
     * 
     * @return void
     */
    void clear();
    
#if defined(CONFIG_KEY_PRESS_EVENT_ENABLED) || defined(CONFIG_KEY_RELEASE_EVENT_ENABLED)
    /**
     * @brief 设置backspace键的键码
     * 
     * @param nKey ...
     * @return void
     */
    void setBackspaceKey ( int nKey );
#endif
    
protected:
#if defined(CONFIG_KEY_PRESS_EVENT_ENABLED)
    virtual bool fwKeyPressEvent ( GKeyEvent * );
#endif
    virtual void paintEvent (  );
#ifdef CONFIG_MOUSE_EVENT_ENABLED
    virtual bool fwMousePressEvent ( GMouseEvent* );
#endif
#ifdef CONFIG_TOUCH_EVENT_ENABLED
    virtual bool fwTapEvent(GTapEvent*);
#endif
    
private:
    DLL_LOCAL void initAttributes ();

private slots:
    DLL_LOCAL void slotTimeOut();
    DLL_LOCAL void slotGetFocus();
    DLL_LOCAL void slotLoseFocus();
    
#ifdef CONFIG_STD_ANSI
    DLL_LOCAL static void slotTimeOut( GObject* p );
    DLL_LOCAL static void slotGetFocus( GObject* p );
    DLL_LOCAL static void slotLoseFocus( GObject* p );
#endif
    
private:
    GCtrlLineEditPrivate *lePriv;
};

#endif

#endif  //CONFIG_gCtrlLineEdit
