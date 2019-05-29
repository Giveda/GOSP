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

#ifdef CONFIG_gCtrlForm

#ifndef GCTRLFORM_H
#define GCTRLFORM_H

#include <gObject.h>
#include <gRect.h>
#include <gColor.h>
#include <gImage.h>
#include <gString.h>

class GMItem;
class GMCtrlItem;
class QCtrlDefaultAppStyle;
#if defined(CONFIG_KEY_PRESS_EVENT_ENABLED) || defined(CONFIG_KEY_RELEASE_EVENT_ENABLED)
class GKeyEvent;
#endif
#ifdef CONFIG_MOUSE_EVENT_ENABLED
class GMouseEvent;
#endif
class GCtrlFormPrivate;
#ifdef CONFIG_TOUCH_EVENT_ENABLED
class GTapEvent;
class GSwipeEvent;
class GGestureScrollEvent;
#endif

/*! @file  gCtrlForm.h
 * @brief  GCtrlForm 窗体、窗口、window。
 * 
 * @author 明心
 * @version 1.0.0
 * @date 2019-2-4
 */

/*!
 * @class GCtrlForm
 * @brief 窗体、窗口、window；GUI软件由若干个窗口组成， GCtrlForm 类代表窗口；窗口可以包含任意数目的控件 GMItem ；如果没有窗口，控件将无法显示。
 * 
 */
class DLL_PUBLIC GCtrlForm : public GObject
{
    SET_CLASS_NAME( GCtrlForm )
    G_DISABLE_COPY ( GCtrlForm, GObject )
    friend class GMItem;
    friend class GMCtrlItem;
public:
    ///在处理键盘按键事件时，窗体能够管理控件焦点，管理控件焦点的模式有手动、自动两种
    enum enumFocusMode 
    {
        ///由窗体自动管理控件焦点，窗体会使用键盘的上下左右方向键来自动切换控件焦点
        Auto_Focus=0,
        
        ///由用户来管理控件焦点，用户须自己处理事件，调用控件的 GMCtrlItem::setFocus 等焦点管理函数
        Manual_Focus=1
    };
#ifdef CONFIG_gWinSys_ENABLE_USR_TOP_FORM
    GCtrlForm ( GCtrlForm* parent=0, const char* name=0, bool top=false );
#else
    /**
     * @brief 构建一个窗体
     * 
     * @param parent ...
     * @param name ...
     */
    GCtrlForm ( GCtrlForm* parent=0, const char* name=0 );
#endif
    virtual ~GCtrlForm();
    
    /**
     * @brief 设置窗体的x坐标
     * 
     * @param x ...
     * @return void
     */
    void setX ( int x );
    
    /**
     * @brief 设置窗体的y坐标
     * 
     * @param y ...
     * @return void
     */
    void setY ( int y );
    
    /**
     * @brief 设置窗体的z坐标
     * 
     * @param z ...
     * @return void
     */
    void setZ ( int z );
    
    /**
     * @brief 设置窗体的宽度
     * 
     * @param w ...
     * @return void
     */
    void setWidth ( int w );
    
    /**
     * @brief 设置窗体的高度
     * 
     * @param h ...
     * @return void
     */
    void setHeight ( int h );
    
    /**
     * @brief 获取窗体的x坐标
     * 
     * @return int
     */
    int x() const;
    
    /**
     * @brief 获取窗体的y坐标
     * 
     * @return int
     */
    int y() const;
    
    /**
     * @brief 获取窗体的z坐标
     * 
     * @return int
     */
    int z() const;
    
    /**
     * @brief 获取窗体的宽度
     * 
     * @return int
     */
    int width () const;
    
    /**
     * @brief 获取窗体的高度
     * 
     * @return int
     */
    int height () const;
    
    /**
     * @brief 获取窗体所占用的矩形区域
     * 
     * @return GRect
     */
    GRect rect() const;
    
    /**
     * @brief 设置窗体的位置
     * 
     * @param x ...
     * @param y ...
     * @return void
     */
    void setPosition ( int x, int y );
    
    /**
     * @brief 设置窗体的大小
     * 
     * @param w ...
     * @param h ...
     * @return void
     */
    void setSize ( int w, int h );
    
    /**
     * @brief 设置窗体所占用的矩形区域，位置及大小
     * 
     * @param x ...
     * @param y ...
     * @param w ...
     * @param h ...
     * @return void
     */
    void setGeometry ( int x, int y, int w, int h );
    
    /**
     * @brief 移动窗体；将窗体沿X轴移动 x 像素，沿Y轴移动 y 像素
     * 
     * @param x ...
     * @param y ...
     * @return void
     */
    void moveBy ( int x, int y );

    /**
     * @brief 设置窗体的背景颜色
     * 
     * @param c ...
     * @return void
     */
    void setPaletteBackgroundColor ( const GColor &c );
    
    /**
     * @brief 加载一个图片作为窗体的背景
     * 
     * @param imgID 图片的数字ID
     * @return void
     */
    void loadBackgroundPixmap ( const uint16_t imgID );
    
    /**
     * @brief 加载一个图片作为窗体的背景
     * 
     * @param px ...
     * @return void
     */
    void loadBackgroundPixmap ( const GImage& px );
    
    /**
     * @brief 让窗体获得焦点；窗体获得焦点后，能够接收到各种输入事件（比如键盘、鼠标、触摸等输入事件）
     * 
     * @return void
     */
    void setFocus();
    
    /**
     * @brief 更新给定的矩形区域；这个函数不会立刻进行重新绘制——而是发送一个绘制事件到事件循环中，后续，事件循环会分发、处理该绘制事件。这样能得到比调用 repaint() 更快的速度和更少的闪烁。
     * 
     * @param r 需要更新的矩形区域
     * @return void
     */
    void update ( GRect r ) ;
    
    /**
     * @brief 更新窗体所在的矩形区域；这个函数不会立刻进行重新绘制——而是发送一个绘制事件到事件循环中，后续，事件循环会分发、处理该绘制事件。这样能得到比调用 repaint() 更快的速度和更少的闪烁。
     * 
     * @return void
     */
    void update();
    
    /**
     * @brief 更新给定的矩形区域；这个函数会立刻进行重新绘制。
     * 
     * @param r 需要更新的矩形区域
     * @return void
     */
    void repaint ( GRect r ) ;
    
    /**
     * @brief 更新窗体所在的矩形区域；这个函数会立刻进行重新绘制
     * 
     * @return void
     */
    void repaint();
    
    /**
     * @brief 返回窗体当前是否可见；show() 之后可见， hide() 之后不可见
     * 
     * @return bool
     */
    bool isVisible ();
    
    /**
     * @brief 设置窗体是否可见；（用户改变窗体的可见属性之后，需要调用 update() 或 repaint() ）
     * 
     * @param b ...
     * @return void
     */
    void setVisible ( bool b );
    
    /**
     * @brief 显示窗体。相当于：\n
     * <pre>
     * setVisible (true);
     * update();
     * </pre>
     * 
     * @return void
     */
    virtual void show();
    
    /**
     * @brief 隐藏窗体。相当于：\n
     * <pre>
     * setVisible (false);
     * update();
     * </pre>
     * 
     * @return void
     */
    virtual void hide();
    
    /**
     * @brief 获取窗体是否具有焦点
     * 
     * @return bool
     */
    bool hasFocus();
    
    /**
     * @brief 设置窗体处理键盘事件时所使用的控件焦点模式
     * 
     * @param mode ...
     * @return void
     */
    void setFocusMode ( enumFocusMode mode );
    
    /**
     * @brief 获取窗体处理键盘事件时所使用的控件焦点模式
     * 
     * @return GCtrlForm::enumFocusMode
     */
    enumFocusMode getFocusMode();
    
    /**
     * @brief 获取窗体焦点是否被禁用；窗体焦点被禁用后，窗体将不能获得焦点 ，不能处理各种输入事件。
     * 
     * @return bool
     */
    bool isFocusEnabled();
    
    /**
     * @brief 设置是否禁用窗体的焦点；窗体焦点被禁用后，窗体将不能获得焦点 ，不能处理各种输入事件。
     * 
     * @param b ...
     * @return void
     */
    void setFocusEnabled ( bool b );
    
    void loseFocus();
    
    void getFocus();
    
    /**
     * @brief 将指定的控件添加到窗体中
     * 
     * @param  ...
     * @return void
     */
    void appendItem ( GMItem* );
    
#if defined(CONFIG_KEY_PRESS_EVENT_ENABLED)
    bool fwKeyPress ( GKeyEvent* e );
#endif
#ifdef CONFIG_MOUSE_EVENT_ENABLED
    bool fwMousePress( GMouseEvent* e );
#endif
#ifdef CONFIG_TOUCH_EVENT_ENABLED
	bool fwTap(GTapEvent* e);
	bool fwSwipe(GSwipeEvent* e);
    bool fwGestureScroll(GGestureScrollEvent* e);
#endif
    
    /**
     * @brief 获取默认的app style
     * 
     * @return QCtrlDefaultAppStyle*
     */
    DLL_LOCAL QCtrlDefaultAppStyle* getDefaultAppStyle();
    
    virtual void paintEvent ();

protected:
#if defined(CONFIG_KEY_PRESS_EVENT_ENABLED)
    virtual bool keyPressEvent ( GKeyEvent* ); //这个函数交由用户实现，当控件不处理按键时，用户可以用这个函数处理按键。
#endif
#ifdef CONFIG_TOUCH_EVENT_ENABLED
	virtual bool tapEvent(GTapEvent*);
    virtual bool swipeEvent ( GSwipeEvent* ); //这个函数交由用户实现。
    virtual bool gestureScrollEvent ( GGestureScrollEvent* ); //这个函数交由用户实现。
#endif
    
    /**
     * @brief 返回当前具有焦点的控件
     * 
     * @return GMCtrlItem*
     */
    GMCtrlItem* getFocusItem();
    
private:
    DLL_LOCAL void initFocus();
    DLL_LOCAL void changeFocus ( GMCtrlItem* pFrom, GMCtrlItem* pTo );
#if defined(CONFIG_KEY_PRESS_EVENT_ENABLED)
    DLL_LOCAL bool fwKeyPressEvent ( GKeyEvent* );
#endif
#ifdef CONFIG_TOUCH_EVENT_ENABLED
	DLL_LOCAL bool fwTapEvent(GTapEvent*);
    DLL_LOCAL bool fwSwipeEvent ( GSwipeEvent* );
    DLL_LOCAL bool fwGestureScrollEvent ( GGestureScrollEvent* );
#endif
    DLL_LOCAL void removePushBack ( GMItem* );
    DLL_LOCAL void inSort ( GMItem* );
    DLL_LOCAL unsigned int getMaxTabIndex();
    
    /**
     * @brief 将焦点设置到指定的控件；让指定的控件来处理各种输入事件
     * 
     * @param pItem ...
     * @return void
     */
    DLL_LOCAL void setFocusToItem ( GMCtrlItem* pItem );
    
private:
    GCtrlFormPrivate *frmPriv;
};

#endif

#endif  //CONFIG_gCtrlForm
