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

#ifdef CONFIG_gCtrlRadioButtonGroup

#ifndef GCTRLRADIOBUTTONGROUP_H
#define GCTRLRADIOBUTTONGROUP_H

#include <gMItem.h>

class GCtrlRadioButtonGroup;
class GCtrlRadioButtonPrivate;

/*! @file  gCtrlRadioButtonGroup.h
 * @brief  GCtrlRadioButton 单选按钮\n
 * GCtrlRadioButtonGroup 将多个单选按钮组织到一个组。
 * 
 * @author 明心
 * @version 1.0.0
 * @date 2019-2-4
 */

/*!
 * @class GCtrlRadioButton
 * @brief 单选按钮。\n
 * 当你需要提供多个选项给用户选择，让用户只能从中选择其一时，你可以创建一个 GCtrlRadioButtonGroup 对象作为一个选项组；选项组可以包含任意数目的选项 GCtrlRadioButton ；如果没有选项组，选项将无法显示。
 * 
 */
class DLL_PUBLIC GCtrlRadioButton : public GMCtrlItem
{
    friend class GCtrlRadioButtonGroup;
    SET_CLASS_NAME( GCtrlRadioButton )
    G_DISABLE_ASSIGN ( GCtrlRadioButton )
    
#ifdef CONFIG_STD_ANSI
    DEFINE_SIGNAL(T_pvrv, checkedChanged)
#else
signals:
    ///当本按钮被选中、或者被取消选中时，该信号将立即发射。注意：初始状态下（程序刚启动时），将不会发射此信号。
    GSignal<void(void)> checkedChanged;
#endif
    
public:
    /**
     * @brief 使用指定的文字标题 str 构造一个单选按钮
     * 
     * @param str 文字标题
     * @param frm ...
     * @param parent ...
     * @param name ...
     * @param check true表示该单选按钮为选中状态，false表示该单选按钮为取消选中状态
     */
    GCtrlRadioButton ( const GString& str, GCtrlForm* frm, GCtrlRadioButtonGroup* parent=0, const char* name=0, bool check=false );
    
    virtual ~GCtrlRadioButton();

    /**
     * @brief 返回该单选按钮是否被选中
     * 
     * @return bool
     */
    bool isChecked();
    
protected:
#if defined(CONFIG_KEY_PRESS_EVENT_ENABLED)
    virtual bool fwKeyPressEvent ( GKeyEvent* );
#endif
    virtual void paintEvent (  );
#ifdef CONFIG_MOUSE_EVENT_ENABLED
    virtual bool fwMousePressEvent ( GMouseEvent* );
#endif
#ifdef CONFIG_TOUCH_EVENT_ENABLED
    virtual bool fwTapEvent(GTapEvent*);
#endif
    
private:
    DLL_LOCAL void checkedChanged2( bool b );

private:
    GCtrlRadioButtonPrivate *rbPriv;
};

class GCtrlRadioButtonGroupPrivate;

class DLL_PUBLIC GCtrlRadioButtonGroup : public GMContainerItem
{
    friend class GCtrlRadioButton;
    SET_CLASS_NAME( GCtrlRadioButtonGroup )
    G_DISABLE_ASSIGN ( GCtrlRadioButtonGroup )
    
#ifdef CONFIG_STD_ANSI
    DEFINE_SIGNAL(T_pirv, checkedChanged)
#else
signals:
    ///当被选中的按钮发生改变时，该信号将立即发射，参数为当前被选中的按钮的索引。注意：初始状态下（程序刚启动时），将不会发射此信号。
    GSignal<void(int)> checkedChanged;
#endif
    
public:
    /**
     * @brief 使用指定的文字标题 tittle 来构造一个单选按钮组
     * 
     * @param tittle 文字标题
     * @param frm ...
     * @param parent ...
     * @param name ...
     */
    GCtrlRadioButtonGroup ( const GString& tittle, GCtrlForm* frm, GMItem* parent=0, const char* name=0 );
    
    virtual ~GCtrlRadioButtonGroup();

    /**
     * @brief 获取文字标题
     * 
     * @return GMText*
     */
    GMText* getTitle();

protected:
#if defined(CONFIG_KEY_PRESS_EVENT_ENABLED)
    virtual bool fwKeyPressEvent ( GKeyEvent* );
#endif
    virtual void paintEvent (  );
#ifdef CONFIG_MOUSE_EVENT_ENABLED
    virtual bool fwMousePressEvent ( GMouseEvent* );
#endif
#ifdef CONFIG_TOUCH_EVENT_ENABLED
    virtual bool fwTapEvent(GTapEvent*);
#endif
    
private slots:
    DLL_LOCAL void slotLoseFocus();
    DLL_LOCAL void slotGetFocus();

private:
    DLL_LOCAL bool moveFocusUp();
    DLL_LOCAL bool moveFocusDown();
    DLL_LOCAL void moveFocus ( int toIndex );
    DLL_LOCAL void checkButton ( GCtrlRadioButton* );
    DLL_LOCAL void appendItem ( GCtrlRadioButton* );
    DLL_LOCAL int index(GCtrlRadioButton* btn) const;
    
private:
    GCtrlRadioButtonGroupPrivate *rbgPriv;
};

#endif

#endif  //CONFIG_gCtrlRadioButtonGroup
