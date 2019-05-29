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

#ifdef CONFIG_gMItem

#ifndef GMITEM_H
#define GMITEM_H

#include <gObject.h>
#include <gFont.h>
#include <gColor.h>
#include <gImage.h>
#include <gPointer.h>
#include <gTimer.h>
#include <gUIEvent.h>
#include <gConstDefine.h>

class GCtrlForm;
class GMItemPrivate;
class QCtrlDefaultAppStyle;

/*! @file  gMItem.h
 * @brief  用于拼装自定义控件的基本元素
 * 
 * @author 明心
 * @version 1.0.0
 * @date 2019-2-6
 */

/*!
 * @class GMItem
 * @brief 所有界面元素的基类
 * 
 */
class DLL_PUBLIC GMItem : public GObject
{
    G_DISABLE_COPY ( GMItem, GObject )
public:
    /**
     * @brief 构造一个基本界面元素
     * 
     * @param form 属于哪个窗体
     * @param parent ...
     * @param name ...
     */
    GMItem ( GCtrlForm* form, GMItem* parent=0, const char* name=0 );
    
    virtual ~GMItem();
    
    /**
     * @brief 设置此界面元素的x坐标
     * 
     * @param x ...
     * @return void
     */
    void setX ( int x );
    
    /**
     * @brief 设置此界面元素的y坐标
     * 
     * @param y ...
     * @return void
     */
    void setY ( int y );
    
    /**
     * @brief 设置此界面元素的z坐标
     * 
     * @param z ...
     * @return void
     */
    void setZ ( int z );
    
    /**
     * @brief 设置此界面元素的宽度
     * 
     * @param w ...
     * @return void
     */
    void setWidth ( int w );
    
    /**
     * @brief 设置此界面元素的高度
     * 
     * @param h ...
     * @return void
     */
    void setHeight ( int h );
    
    /**
     * @brief 获取此界面元素的x坐标
     * 
     * @return int
     */
    int x() const;
    
    /**
     * @brief 获取此界面元素的y坐标
     * 
     * @return int
     */
    int y() const;
    
    /**
     * @brief 获取此界面元素的z坐标
     * 
     * @return int
     */
    int z() const;
    
    /**
     * @brief 获取此界面元素的宽度
     * 
     * @return int
     */
    int width () const;
    
    /**
     * @brief 获取此界面元素的高度
     * 
     * @return int
     */
    int height () const;
    
    /**
     * @brief 获取右边界的坐标
     * 
     * @return int
     */
    int right () const;
    
    /**
     * @brief 获取下边界的坐标
     * 
     * @return int
     */
    int bottom() const;
    
    /**
     * @brief 获取此界面元素所占用的矩形区域
     * 
     * @return GRect
     */
    GRect rect() const;
    
    /**
     * @brief 设置此界面元素的位置
     * 
     * @param x ...
     * @param y ...
     * @return void
     */
    void setPosition ( int x, int y );
    
    /**
     * @brief 设置此界面元素的大小
     * 
     * @param w ...
     * @param h ...
     * @return void
     */
    void setSize ( int w, int h );
    
    /**
     * @brief 设置此界面元素的大小
     * 
     * @param s ...
     * @return void
     */
    void setSize ( const GSize &s );
    
    /**
     * @brief 设置此界面元素所占用的矩形区域，位置及大小
     * 
     * @param x ...
     * @param y ...
     * @param w ...
     * @param h ...
     * @return void
     */
    void setGeometry ( int x, int y, int w, int h );
    
    /**
     * @brief 移动此界面元素；将此界面元素沿X轴移动 x 像素，沿Y轴移动 y 像素
     * 
     * @param x ...
     * @param y ...
     * @return void
     */
    void moveBy ( int x, int y );
    
    /**
     * @brief 设置此界面元素的背景颜色
     * 
     * @param c ...
     * @return void
     */
    void setPaletteBackgroundColor ( const GColor &c );
    
    /**
     * @brief 加载一个图片作为此界面元素的背景
     * 
     * @param imgID 图片的数字ID
     * @return void
     */
    void loadBackgroundPixmap ( const uint16_t imgID );
    
    /**
     * @brief 返回此界面元素当前是否可见；show() 之后可见， hide() 之后不可见
     * 
     * @return bool
     */
    bool isVisible ();
    
    /**
     * @brief 设置此界面元素是否可见；（用户改变此界面元素的可见属性之后，需要调用 update() 或 repaint() ）
     * 
     * @param b ...
     * @return void
     */
    void setVisible ( bool b );
    
    /**
     * @brief 显示此界面元素。相当于：\n
     * <pre>
     * setVisible (true);
     * update();
     * </pre>
     * 
     * @return void
     */
    virtual void show();
    
    /**
     * @brief 隐藏此界面元素。相当于：\n
     * <pre>
     * setVisible (false);
     * update();
     * </pre>
     * 
     * @return void
     */
    virtual void hide();
    
    /**
     * @brief 是否可以接收输入事件
     * 
     * @return bool
     */
    virtual bool isCtrlItem();
    
    
    virtual bool isContainerItem();
    
    /**
     * @brief 获取此界面元素所在的窗体
     * 
     * @return GCtrlForm*
     */
    GCtrlForm* form();
    
    /**
     * @brief 更新此界面元素所在的矩形区域；这个函数不会立刻进行重新绘制——而是发送一个绘制事件到事件循环中，后续，事件循环会分发、处理该绘制事件。这样能得到比调用 repaint() 更快的速度和更少的闪烁。
     * 
     * @return void
     */
    void update();
    
public:
    virtual void draw ( );
    QCtrlDefaultAppStyle* getDefaultAppStyle();
    virtual bool isComplex();
    
protected:
    virtual void paintEvent ( );
    
private:
    GMItemPrivate *iPriv;
};

class GMCtrlItemPrivate;

/*!
 * @class GMCtrlItem
 * @brief 可以接收输入事件的界面元素的基类
 * 
 */
class DLL_PUBLIC GMCtrlItem : public GMItem
{
    friend class GCtrlForm;
    friend class GMContainerItem;
    G_DISABLE_ASSIGN ( GMCtrlItem )
#ifndef CONFIG_STD_CPP11
    DEFINE_SIGNAL(T_pvrv, loseFocus)
    DEFINE_SIGNAL(T_pvrv, getFocus)
#else
signals:
    ///当此元素失去焦点时，会立即发射此信号
    GSignal<void(void)>  loseFocus;
    
    ///当此元素获得焦点时，会立即发射此信号
    GSignal<void(void)>  getFocus;
#endif

public:
    /**
     * @brief 构造一个可以接收输入事件的界面元素
     * 
     * @param form 属于哪个窗体
     * @param parent ...
     * @param name ...
     */
    GMCtrlItem ( GCtrlForm* form, GMItem* parent=0, const char* name=0 );
    
    virtual ~GMCtrlItem();
    
    /**
     * @brief 是否可以接收输入事件
     * 
     * @return bool
     */
    virtual bool isCtrlItem();
    
    /**
     * @brief 获取焦点是否被禁用；焦点被禁用后，此元素将不能获得焦点 ，不能处理各种输入事件。
     * 
     * @return bool
     */
    bool isFocusEnabled();
    
    /**
     * @brief 设置是否禁此元素的焦点；焦点被禁用后，此元素将不能获得焦点 ，不能处理各种输入事件。
     * 
     * @param b ...
     * @return void
     */
    void setFocusEnabled ( bool b );
    
    /**
     * @brief 设置此元素到有焦点状态
     * 
     * @return bool
     */
    void setFocus();
    
    /**
     * @brief 返回此元素是否具有焦点
     * 
     * @return bool
     */
    bool hasFocus();
    
    /**
     * @brief 设置此界面元素的tab index；在使用键盘来操作界面时，需要用到tab index
     * 
     * @param index ...
     * @return void
     */
    void setTabIndex ( unsigned int index );
    
    /**
     * @brief 获取此界面元素的tab index；在使用键盘来操作界面时，需要用到tab index
     * 
     * @return unsigned int
     */
    unsigned int tabIndex();
    
    /**
     * @brief 隐藏此界面元素。相当于：\n
     * <pre>
     * setVisible (false);
     * update();
     * </pre>
     * 
     * @return void
     */
    virtual void hide ();
    
protected:
#if defined(CONFIG_KEY_PRESS_EVENT_ENABLED)
    /**
     * @brief 用户可以在子类中覆盖这个函数处理按键。用户不可以覆盖任何以fw开头的函数
     * 
     * @param  ...
     * @return bool
     */
    virtual bool keyPressEvent ( GKeyEvent* );
    
    virtual bool fwKeyPressEvent ( GKeyEvent* );
#endif
    
#ifdef CONFIG_MOUSE_EVENT_ENABLED
    /**
     * @brief 用户可以在子类中覆盖这个函数处理鼠标事件。用户不可以覆盖任何以fw开头的函数
     * 
     * @param  ...
     * @return bool
     */
    virtual bool mousePressEvent ( GMouseEvent* );
    
    virtual bool fwMousePressEvent ( GMouseEvent* );
#endif
    
#ifdef CONFIG_TOUCH_EVENT_ENABLED
    /**
     * @brief 用户可以在子类中覆盖这个函数处理点击事件。用户不可以覆盖任何以fw开头的函数
     * 
     * @param  ...
     * @return bool
     */
    virtual bool tapEvent(GTapEvent*);
        
    /**
     * @brief 用户可以在子类中覆盖这个函数处理滑动事件。用户不可以覆盖任何以fw开头的函数
     * 
     * @param  ...
     * @return bool
     */
    virtual bool swipeEvent ( GSwipeEvent* );
    
    /**
     * @brief 用户可以在子类中覆盖这个函数处理滑动手势。用户不可以覆盖任何以fw开头的函数
     * 
     * @param  ...
     * @return bool
     */
    virtual bool gestureScrollEvent ( GGestureScrollEvent* );
    
    virtual bool fwGestureScrollEvent ( GGestureScrollEvent* );
    virtual bool fwTapEvent(GTapEvent*);
    virtual bool fwSwipeEvent ( GSwipeEvent* );
#endif
    
public:
#if defined(CONFIG_KEY_PRESS_EVENT_ENABLED)
    bool fwKeyPress ( GKeyEvent * e);
#endif
    
#ifdef CONFIG_MOUSE_EVENT_ENABLED
    bool fwMousePress( GMouseEvent* e );
#endif
    
#ifdef CONFIG_TOUCH_EVENT_ENABLED
    bool fwTap(GTapEvent* e);
    bool fwSwipe( GSwipeEvent* e);
    bool fwGestureScroll( GGestureScrollEvent* e);
#endif

private:
    DLL_LOCAL void emitLoseFocus();
    DLL_LOCAL void emitGetFocus();
    
private:
    GMCtrlItemPrivate *ciPriv;
};

/*!
 * @class GMContainerItem
 * @brief 所有容器元素的基类；一个容器元素可以包含任意数目的 GMItem 对象
 * 
 */
class DLL_PUBLIC GMContainerItem : public GMCtrlItem
{
    G_DISABLE_ASSIGN ( GMContainerItem )
public:
    /**
     * @brief 构造一个能够包含任意数目 GMItem 的窗口界面元素
     * 
     * @param form 属于哪个窗体
     * @param parent ...
     * @param name ...
     */
    GMContainerItem ( GCtrlForm* form, GMItem* parent=0, const char* name=0 );
    
    /**
     * @brief 设置焦点到指定的 GMCtrlItem 
     * 
     * @param pItem ...
     * @return void
     */
    void setFocusToItem ( GMCtrlItem* pItem );
    
    /**
     * @brief 是否是容器
     * 
     * @return bool
     */
    virtual bool isContainerItem();
    
protected:
    GPointer<GMCtrlItem> mpFocus;
};

class GMPixmapPrivate;

/*!
 * @class GMPixmap
 * @brief 图片元素（原样显示，不会自动缩放到用户所设置的尺寸大小）；\n
 * GCtrlPixmap 与 GMPixmap 的区别在于： GCtrlPixmap 会主动将自己添加到 GCtrlForm 中，而 GMPixmap 则不会；因此开发者可以直接在 GCtrlForm 中使用 GCtrlPixmap ；而 GMPixmap 则可以用于拼装出自定义的UI控件。
 * 
 */
class DLL_PUBLIC GMPixmap : public GMItem
{
    G_DISABLE_ASSIGN ( GMPixmap )
public:
    /**
     * @brief 构造一个空的图片元素
     * 
     * @param form ...
     * @param parent ...
     * @param name ...
     */
    GMPixmap ( GCtrlForm* form, GMItem* parent=0, const char* name=0 );
    
    /**
     * @brief 使用指定的图片ID去构造一个图片元素
     * 
     * @param imgID 图片ID
     * @param form ...
     * @param parent ...
     * @param name ...
     */
    GMPixmap ( const uint16_t imgID, GCtrlForm* form, GMItem* parent = 0, const char* name = 0 );
    
    /**
     * @brief 使用指定的图片对象去构造一个图片元素
     * 
     * @param img 图片
     * @param form ...
     * @param parent ...
     * @param name ...
     */
    GMPixmap ( const GImage & img, GCtrlForm* form, GMItem* parent=0, const char* name=0 );
    
    virtual ~GMPixmap();
    
    /**
     * @brief 使用指定的数字ID去加载图片。此控件的大小将被设置为图片的大小
     * 
     * @param imgID ...
     * @return 
     */
    void load ( const uint16_t imgID );
    
    /**
     * @brief 加载指定的图片对象。此控件的大小将被设置为图片的大小
     * 
     * @param pm ...
     * @return 
     */
    void setPixmap ( const GImage &pm );
    
    /**
     * @brief 使用指定的数字ID去加载图片。但不会设置此控件的大小到图片大小
     * 
     * @param imgID ...
     * @return 
     */
    void loadButNotAdjustSize ( const uint16_t imgID );
    
    /**
     * @brief 加载指定的图片对象。但不会设置此控件的大小到图片大小
     * 
     * @param pm ...
     * @return 
     */
    void setPixmapButNotAdjustSize ( const GImage &pm );
    
protected:
    virtual void paintEvent (  );
    
private:
    GMPixmapPrivate *pixPriv;
};

class GMImagePrivate;

/*!
 * @class GMImage
 * @brief 图片元素（能够自动缩放到用户所设置的尺寸大小）；\n
 * GCtrlImage 与 GMImage 的区别在于： GCtrlImage 会主动将自己添加到 GCtrlForm 中，而 GMImage 则没有；因此开发者可以直接在 GCtrlForm 中使用 GCtrlImage ；而 GMImage 则可以用于拼装出自定义的UI控件。
 * 
 */
class DLL_PUBLIC GMImage : public GMItem
{
    G_DISABLE_ASSIGN ( GMImage )
public:
    /**
     * @brief 构造一个空的图片元素
     * 
     * @param form ...
     * @param parent ...
     * @param name ...
     */
    GMImage ( GCtrlForm* form, GMItem* parent=0, const char* name=0 );
    
    /**
     * @brief 使用指定的图片ID去构造一个图片元素
     * 
     * @param imgID 图片ID
     * @param form ...
     * @param parent ...
     * @param name ...
     */
    GMImage ( const uint16_t imgID, GCtrlForm* form, GMItem* parent = 0, const char* name = 0 );
    
    /**
     * @brief 使用指定的图片对象去构造一个图片元素
     * 
     * @param img 图片
     * @param form ...
     * @param parent ...
     * @param name ...
     */
    GMImage ( const GImage & img, GCtrlForm* form, GMItem* parent=0, const char* name=0 );
    
    virtual ~GMImage();
    
    /**
     * @brief 使用指定的数字ID去加载图片。此控件的大小将被设置为图片的大小
     * 
     * @param imgID ...
     * @return 
     */
    void load ( const uint16_t imgID );
    
    /**
     * @brief 加载指定的图片对象。此控件的大小将被设置为图片的大小
     * 
     * @param img ...
     * @return 
     */
    void setImage ( const GImage &img );

protected:
    virtual void paintEvent ();
    
private:
    GMImagePrivate *imgPriv;
};

class GMTextPrivate;

/*!
 * @class GMText
 * @brief 文字元素；\n
 * GCtrlText 与 GMText 的区别在于： GCtrlText 会主动将自己添加到 GCtrlForm 中，而 GMText 则没有；因此开发者可以直接在 GCtrlForm 中使用 GCtrlText ；而 GMText 则可以用于拼装出自定义的UI控件。
 * 
 */
class DLL_PUBLIC GMText : public GMItem
{
    G_DISABLE_ASSIGN ( GMText )
public:
    /**
     * @brief 构造一个空的 GMText 文字元素
     * 
     * @param form ...
     * @param parent ...
     * @param name ...
     */
    GMText ( GCtrlForm* form, GMItem* parent=0, const char* name=0 );
    
    /**
     * @brief 使用开发者指定的字符串去构造一个 GMText 文字元素
     * 
     * @param str 字符串
     * @param form ...
     * @param parent ...
     * @param name ...
     */
    GMText ( const GString & str, GCtrlForm* form, GMItem* parent=0, const char* name=0 );
    
    virtual ~GMText();
    
    /**
     * @brief 使用指定的文本内容
     * 
     * @param str ...
     * @return void
     */
    void setText ( const GString& str );
    
    /**
     * @brief 使用指定的字体和字号去显示文本
     * 
     * @param font ...
     * @return void
     */
    void setFont ( const GFont & font );
    
    /**
     * @brief 使用指定的颜色去显示文本
     * 
     * @param color ...
     * @return void
     */
    void setColor ( const GColor& color );
    
    /**
     * @brief 设置排版格式。nFlags可由 Giveda::AlignmentFlag 和 Giveda::TextFlag 进行位操作（比如进行按位相或）得到。
     * 
     * @param nFlags ...
     * @return void
     */
    void setTextFlags ( int nFlags );
    
    /**
     * @brief 返回当前的文本内容
     * 
     * @return GString
     */
    GString text();
    
    /**
     * @brief 获取单行显示情况下的宽高
     * 
     * @return GSize
     */
    GSize getSingleLineSize();
    
    /**
     * @brief 获取当前的字体
     * 
     * @return GFont
     */
    GFont font();
    
protected:
    virtual void paintEvent ();
    
protected://这里用protected，GMScrollText中访问比较方便
    GMTextPrivate *txtPriv;
};

class GMScrollTextPrivate;

/*!
 * @class GMScrollText
 * @brief 滚动字幕；\n
 * GCtrlScrollText 与 GMScrollText 的区别在于： GCtrlScrollText 会主动将自己添加到 GCtrlForm 中，而 GMScrollText 则没有；因此开发者可以直接在 GCtrlForm 中使用 GCtrlScrollText ；而 GMScrollText 则可以用于拼装出自定义的UI控件。
 * 
 */
class DLL_PUBLIC GMScrollText : public GMText
{
    G_DISABLE_ASSIGN ( GMScrollText )
public:
     /**
     * @brief 构造一个空的 滚动字幕
     * 
     * @param form ...
     * @param parent ...
     * @param name ...
     */
    GMScrollText ( GCtrlForm* form, GMItem* parent=0, const char* name=0 );
    
    /**
     * @brief 使用开发者指定的字符串去构造一个 滚动字幕
     * 
     * @param str 字符串
     * @param form ...
     * @param parent ...
     * @param name ...
     */
    GMScrollText ( const GString & str, GCtrlForm* form, GMItem* parent=0, const char* name=0 );
    
    virtual ~GMScrollText();

    /**
     * @brief 开始滚动。刷新的时间间隔为 msec 
     * 
     * @param msec ...
     * @return void
     */
    void startScroll ( unsigned int msec=500 );
    
    /**
     * @brief 停止滚动
     * 
     * @return void
     */
    void stopScroll();
    
    /**
     * @brief 返回当前是否正在滚动显示
     * 
     * @return bool
     */
    bool isScroll();
    
protected:
    virtual void paintEvent ();
    
private:
    DLL_LOCAL bool isNeedScroll();
    
#ifndef CONFIG_STD_CPP11
private slots:
    DLL_LOCAL void slotUpdate();
    DLL_LOCAL static void slotUpdate( GObject* );
#else
private slots:
    DLL_LOCAL void slotUpdate();
#endif
    
private:
    GMScrollTextPrivate *stPriv;
};

#endif

#endif  //CONFIG_gMItem
