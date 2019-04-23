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

#ifdef CONFIG_gCtrlListBox

#ifndef GCTRLLISTBOX_H
#define GCTRLLISTBOX_H

#include <gMItem.h>

class GCtrlListBox;

/*! @file  gCtrlListBox.h
 * @brief  GCtrlListBox windows资源管理器中的列表视图。
 * 
 * @author 明心
 * @version 1.0.0
 * @date 2019-2-4
 */

/*!
 * @class GCtrlListBoxItem
 * @brief 列表视图中可以包含任意数目的列表项， GCtrlListBoxItem 类代表只有一个文字标题的所述列表项；
 * 
 */
class DLL_PUBLIC GCtrlListBoxItem : public GMCtrlItem
{
    G_DISABLE_ASSIGN ( GCtrlListBoxItem )
public:
    /**
     * @brief 构造一个列表项；此列表项须要与列表视图 GCtrlListBox 配套使用。
     * 
     * @param str 图标的标题
     * @param frm ...
     * @param parent 此列表项需要放置到哪个列表视图
     * @param name ...
     */
    GCtrlListBoxItem ( const GString& str, GCtrlForm* frm, GCtrlListBox* parent=0, const char* name="listBoxItem" );
    virtual ~GCtrlListBoxItem();
    
    /**
     * @brief 获取文字标题
     * 
     * @return GMScrollText*
     */
    GMScrollText* getTxt();
    
    /**
     * @brief 获取文字标题的内容
     * 
     * @return GString
     */
    GString text();
    
    /**
     * @brief 设置列表项所占用的矩形区域，位置及大小
     * 
     * @param x ...
     * @param y ...
     * @param w ...
     * @param h ...
     * @return void
     */
    virtual void setGeometry ( int x, int y, int w, int h  );
    
protected:
#if defined(CONFIG_KEY_PRESS_EVENT_ENABLED)
    virtual bool fwKeyPressEvent ( GKeyEvent* );
#endif
    virtual void paintEvent (  );
    
protected:
    GMScrollText m_txt;
};

/*!
 * @class GCtrlListBoxPixmap
 * @brief 列表视图中可以包含任意数目的列表项， GCtrlListBoxPixmap 类代表拥有一个图标、和一个文字标题的所述列表项；
 * 
 */
class DLL_PUBLIC GCtrlListBoxPixmap : public GCtrlListBoxItem
{
    G_DISABLE_ASSIGN ( GCtrlListBoxPixmap )
public:
    /**
     * @brief 构造一个列表项；此列表项须要与列表视图 GCtrlListBox 配套使用。
     * 
     * @param pm 列表项的图片
     * @param str 列表项的文字标题
     * @param frm 属于哪个窗体
     * @param parent 此列表项需要放置到哪个列表视图
     * @param name ...
     */
    GCtrlListBoxPixmap ( const GImage& pm, const GString& str, GCtrlForm* frm, GCtrlListBox* parent = 0, const char* name = "listBoxPixmap" );
    
    /**
     * @brief 设置列表项所占用的矩形区域，位置及大小
     * 
     * @param x ...
     * @param y ...
     * @param w ...
     * @param h ...
     * @return void
     */
    virtual void setGeometry ( int x, int y, int w, int h  );
    
protected:
    virtual void paintEvent (  );
    
protected:
    GMPixmap m_pix;
};

#ifdef CONFIG_gCtrlIconView
/*!
 * @class GCtrlIconViewItem
 * @brief 列表视图中可以包含任意数目的列表项， GCtrlIconViewItem 类代表了图标在上、文字在下的所述列表项；
 * 
 */
class DLL_PUBLIC GCtrlIconViewItem : public GCtrlListBoxPixmap
{
    G_DISABLE_ASSIGN ( GCtrlIconViewItem )
public:
    /**
     * @brief 构造一个图标在上、文字在下的列表项，并通过 showTitle 来控制是否显示文字标题；此列表项须要与列表视图 GCtrlListBox 配套使用。
     * 
     * @param img 列表项的图片
     * @param str 列表项的文字标题
     * @param frm 属于哪个窗体
     * @param parent 此列表项需要放置到哪个列表视图
     * @param name ...
     * @param showTitle 是否显示文字标题
     */
    GCtrlIconViewItem ( const GImage& img, const GString& str, GCtrlForm* frm, GCtrlListBox* parent, const char* name="iconViewItem", const bool showTitle=false );
    
    /**
     * @brief 设置列表项所占用的矩形区域，位置及大小
     * 
     * @param x ...
     * @param y ...
     * @param w ...
     * @param h ...
     * @return void
     */
    virtual void setGeometry ( int x, int y, int w, int h  );
};
#endif

class GCtrlListBoxPrivate;

/*!
 * @class GCtrlListBox
 * @brief GCtrlListBox 类提供了类似windows资源管理器中的列表视图；列表视图可以包含任意数目的列表项 GCtrlListBoxItem ；如果没有列表视图，列表项将无法显示。
 * 
 */
class DLL_PUBLIC GCtrlListBox : public GMContainerItem
{
    friend class GCtrlListBoxItem;
    SET_CLASS_NAME( GCtrlListBox )
    G_DISABLE_ASSIGN ( GCtrlListBox )
#ifdef CONFIG_STD_ANSI
public:
    typedef void (*T_pIrv)( GObject*p, const GCtrlListBoxItem* );
    DEFINE_SIGNAL( T_pIrv, focusChangedTo_pI)
    DEFINE_SIGNAL( T_pirv, focusChangedTo_pi)
    DEFINE_SIGNAL( T_pIrv, highlighted_pI)
    DEFINE_SIGNAL( T_pirv, highlighted_pi)
    DEFINE_SIGNAL( T_pIrv, selected_pI)
    DEFINE_SIGNAL( T_pirv, selected_pi)
#else
signals:
    ///当列表项的焦点发生改变时，该信号被立即发射；参数为：当前具有焦点的列表项
    GSignal<void ( const GCtrlListBoxItem* )> focusChangedTo_pI;
    
    ///当列表项的焦点发生改变时，该信号被立即发射；参数为：当前具有焦点的列表项的索引值
    GSignal<void ( int )> focusChangedTo_pi;
    
    ///列表项的焦点发生了改变，并且10秒之内列表项的焦点没有再次发生改变，该信号被立即发射；参数为：当前具有焦点的列表项
    GSignal<void ( const GCtrlListBoxItem* )> highlighted_pI;
    
    ///列表项的焦点发生了改变，并且10秒之内列表项的焦点没有再次发生改变，该信号被立即发射；参数为：当前具有焦点的列表项的索引值
    GSignal<void ( int )> highlighted_pi;
    
    ///当列表项被用户点击时，该信号被立即发射；参数为：当前具有焦点的列表项
    GSignal<void ( const GCtrlListBoxItem* )> selected_pI;
    
    ///当列表项被用户点击时，该信号被立即发射；参数为：当前具有焦点的列表项的索引值
    GSignal<void ( int )> selected_pi;
#endif

public:
    ///列表如何排版其中的列表项
    enum E_ITEMS_LAYOUT_MODE
    {
        ///默认值。从上往下排版列表项，超出下边界则在右侧另起一列
        E_ITEMS_LAYOUT_TOP_2_BOTTOM=0,
        
        ///从左向右排版列表项，超出右边界则在下方另起一行
        E_ITEMS_LAYOUT_LEFT_2_RIGHT,
    };
    
    /**
     * @brief 构造一个列表视图。一个列表视图可以包含任意数目的列表项 GCtrlListBoxItem 。默认情况下，列表视图会按照列表项被添加进来的先后顺序，为所有列表项建立索引。用户可以使用索引值来访问、操作任一列表项。默认情况下，列表使用 E_ITEMS_LAYOUT_TOP_2_BOTTOM 从下往下依次布局列表项。
     * 
     * @param frm ...
     * @param parent ...
     * @param name ...
     * @param showSingleTitle 是否仅显示当前具有焦点的图标项的文字标题。如果为true，则其它无焦点的图标项的文字标题会被隐藏。
     */
    GCtrlListBox ( GCtrlForm* frm, GMItem* parent=0, const char* name=0, const bool showSingleTitle=false  );
    
    virtual ~GCtrlListBox();
    
    /**
     * @brief 向列表视图中添加列表项。列表视图会按照列表项被添加进来的先后顺序，为所有列表项建立索引。
     * 
     * @param  p  列表项
     * @param index 指定列表项在列表视图中的显示位置；-1意味着列表项将被放置在列表视图的最后；
     * @return void
     */
    void insertItem ( GCtrlListBoxItem* p, int index=-1 );
    
    /**
     * @brief 从列表视图中删除一个列表项。如果 index 等于-1，会删除当前处于焦点状态的列表项；否则，删除第 index 个列表项。系统将自动回收被删除掉的列表项所占用的内存。
     * 
     * @param index ...
     * @return void
     */
    void removeItem ( int index=-1 );
    
    /**
     * @brief 从列表视图中删除指定的列表项。系统将自动回收被删除掉的列表项所占用的内存。
     * 
     * @param  ...
     * @return void
     */
    void removeItem ( GCtrlListBoxItem* );
    
    /**
     * @brief 更换第 index 个列表项；系统将自动回收被替换掉的列表项所占用的内存；如果 index 等于-1，会替换掉当前处于焦点状态的列表项；
     * 
     * @param  ...
     * @param index ...
     * @return void
     */
    void changeItem ( GCtrlListBoxItem *, int index=-1 );
    
    /**
     * @brief 设置列表视图的行数，即指定一下列表视图中包含几行列表项
     * 
     * @param num ...
     * @return void
     */
    void setRowNums ( unsigned int num );
    
    /**
     * @brief 设置列表视图的列数，即指定一行中包含几个列表项
     * 
     * @param num ...
     * @return void
     */
    void setColumnNums ( unsigned int num );
    
    /**
     * @brief 获取第 index 个列表项；如果index=-1则获取当前具有焦点状态的列表项
     * 
     * @param index ...
     * @return GCtrlListBoxItem*
     */
    GCtrlListBoxItem* item ( int index=-1 );
    
    /**
     * @brief 获取当前具有焦点状态的列表项的索引值
     * 
     * @return int
     */
    int getCurItemIndex();
    
    /**
     * @brief 返回当前列表视图中所包含的列表项的数目
     * 
     * @return unsigned int
     */
    unsigned int count();
    
    /**
     * @brief 清空列表视图。列表视图中包含的所有列表项将被删除，它们占用的内存空间将被收回。
     * 
     * @return void
     */
    void clear();
    
    /**
     * @brief 设置当列表视图处于焦点状态时，列表项所使用的焦点图片
     * 
     * @param  ...
     * @return void
     */
    void setFocusInImage ( const GImage& );
    
    /**
     * @brief 设置当列表视图失去焦点状态之后，列表项所使用的焦点图片
     * 
     * @param  ...
     * @return void
     */
    void setFocusOutImage ( const GImage& );
    
    /**
     * @brief 让第 nIndex 个列表项得到焦点状态
     * 
     * @param nIndex ...
     * @return void
     */
    void setCurItemIndex ( const int nIndex );

    /**
     * @brief 设置列表项文字标题的颜色
     * 
     * @param c ...
     * @return void
     */
    void setItemTextColor ( const GColor& c );
    
    /**
     * @brief 设置列表项文字标题的字体
     * 
     * @param f ...
     * @return void
     */
    void setItemTextFont ( const GFont& f );
    
    /**
     * @brief 获得列表项文字标题的颜色
     * 
     * @return GColor
     */
    GColor getItemTextColor();
    
    /**
     * @brief 获得列表项文字标题的字体
     * 
     * @return GFont
     */
    GFont getItemTextFont();
    
    /**
     * @brief 设置列表项的文字标题（在标题长度过长的情况下）是否能够以滚动字幕的形式进行显示
     * 
     * @param enabled ...
     * @return void
     */
    void setItemScrollEnabled ( bool enabled );
    
    /**
     * @brief 设置列表应当如何排版列表项。默认情况下，列表使用 E_ITEMS_LAYOUT_TOP_2_BOTTOM 从下往下依次布局列表项
     * 
     * @param mode ...
     * @return void
     */
    void setLayoutMode( E_ITEMS_LAYOUT_MODE mode );
    
    /**
     * @brief 获取列表的布局模式。默认情况下，列表使用 E_ITEMS_LAYOUT_TOP_2_BOTTOM 从下往下依次布局列表项
     * 
     * @return GCtrlListBox::E_ITEMS_LAYOUT_MODE
     */
    GCtrlListBox::E_ITEMS_LAYOUT_MODE getLayoutMode() const;
    
#if defined(CONFIG_KEY_EVENT_ENABLED)
    /**
     * @brief 设置一个用于将焦点移动到上一个列表项的按键键值。默认值为 Giveda::Key_Up \n
     * 请注意，因为PreviousItemKey/NextItemKey/PreviousColumnKey/NextColumnKey都有默认的值，所以调用此函数有可能会导致键值冲突。键值冲突时，框架会打印出来一条warning提示：can not override another key
     * 
     * @param nKey ...
     * @return void
     */
    void setPreviousItemKey ( int nKey );
    
    /**
     * @brief 设置一个用于将焦点移动到下一个列表项的按键键值。默认值为 Giveda::Key_Down \n
     * 请注意，因为PreviousItemKey/NextItemKey/PreviousColumnKey/NextColumnKey都有默认的值，所以调用此函数有可能会导致键值冲突。键值冲突时，框架会打印出来一条warning提示：can not override another key
     * 
     * @param nKey ...
     * @return void
     */
    void setNextItemKey ( int nKey );
    
    /**
     * @brief 设置是否允许循环切换焦点；\n
     * 如果允许，则当移到焦点到最后一个列表项后，此时如果试图移动焦点到下一个列表项，则焦点将移动到第一个列表项；\n
     * 如果禁止，则当移到焦点到最后一个列表项后，此时如果试图移动焦点到下一个列表项，则焦点保持不变。
     * 
     * @param b ...
     * @return void
     */
    void setLoopFocus ( bool b );
    
        /**
     * @brief 设置一个用于将焦点向前移动一列的按键键值。默认值为 Giveda::Key_PageUp \n
     * 请注意，因为PreviousItemKey/NextItemKey/PreviousColumnKey/NextColumnKey都有默认的值，所以调用此函数有可能会导致键值冲突。键值冲突时，框架会打印出来一条warning提示：can not override another key
     * 
     * @param nKey ...
     * @return void
     */
    void setPreviousColumnKey ( int nKey );
    
    /**
     * @brief 设置一个用于将焦点向后移动一列的按键键值。默认值为 Giveda::Key_PageDown \n
     * 请注意，因为PreviousItemKey/NextItemKey/PreviousColumnKey/NextColumnKey都有默认的值，所以调用此函数有可能会导致键值冲突。键值冲突时，框架会打印出来一条warning提示：can not override another key
     * 
     * @param nKey ...
     * @return void
     */
    void setNextColumnKey ( int nKey );
#endif
    
protected:
#if defined(CONFIG_KEY_PRESS_EVENT_ENABLED)
    virtual bool fwKeyPressEvent ( GKeyEvent* );
#endif
#ifdef CONFIG_MOUSE_EVENT_ENABLED
    virtual bool fwMousePressEvent ( GMouseEvent* );
#endif
#ifdef CONFIG_TOUCH_EVENT_ENABLED
    virtual bool fwTapEvent(GTapEvent*);
    virtual bool fwSwipeEvent ( GSwipeEvent* );
#endif
    virtual void paintEvent ();

    /**
     * @brief 获取列表项的最大显示高度
     * 
     * @return int
     */
    int getMaxIconHeight();
    
    /**
     * @brief 获取列表项的最大显示宽度
     * 
     * @return int
     */
    int getMaxIconWidth();
    
private slots:
    DLL_LOCAL void slotFocusChangedTo();
    DLL_LOCAL void slotLoseFocus();
    DLL_LOCAL void slotGetFocus();
    
#ifdef CONFIG_STD_ANSI
private slots:
    DLL_LOCAL static void slotFocusChangedTo( GObject* p );
    DLL_LOCAL static void slotLoseFocus( GObject* p );
    DLL_LOCAL static void slotGetFocus( GObject* p );
#endif
    
private:
#if defined(CONFIG_KEY_EVENT_ENABLED)
    DLL_LOCAL bool moveFocusLeft();
    DLL_LOCAL bool moveFocusRight();
    DLL_LOCAL bool moveFocusUp();
    DLL_LOCAL bool moveFocusDown();
#endif
    DLL_LOCAL void moveFocus ( int toIndex );

    DLL_LOCAL void emitSelected ( GCtrlListBoxItem* );

private:
    GCtrlListBoxPrivate *lbPriv;
};

#endif

#endif  //CONFIG_gCtrlListBox
