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

#ifdef CONFIG_gUIEvtLoop

#ifndef GUIEVTLOOP_H
#define GUIEVTLOOP_H

#include <gUIEvent.h>
#include <gObject.h>

class GUIEvtLoopPriv;
class GImageAttr;

/*! @file  gUIEvtLoop.h
 * @brief  GUIEvtLoop 为GUI程序提供了一个事件循环。
 */

/*!
 * @class GUIEvtLoop
 * @brief 通常情况下，GUI程序都是由事件驱动的、需要有一个事件循环。 GUIEvtLoop 为GUI程序提供了一个事件循环。\n
 * GUIEvtLoop 管理着GUI程序的执行流程、负责GUI程序在总体上、全局上的基本设置。一个GUI程序应当有且仅有一个 GUIEvtLoop 对象。\n
 * 在你软件代码的任何地方，你可以使用 \c uiEvtLp 这个全局变量来访问 GUIEvtLoop 对象。
 * 作为一个最佳实践，用户可以创建一个 GUIEvtLoop 的子类（比如命名为GUIApplication），然后把所有事关软件总体、和全局的初始化等操作放在这个GUIApplication子类中。\n
 * 
 * @author 明心
 * @version 1.0.0
 * @date 2019-2-4
 */
class DLL_PUBLIC GUIEvtLoop : public GObject
{
public:
    /**
     * @brief 使用argv和argc作为命令行参数，构造一个GUI事件循环对象。举例如下：\n
     * <pre>
     * char* param[] = {
     *     NULL, //这个位置放置的是程序的文件名，直接置为NULL也是可以的
     *     (char*)"gCtrlMsgBox", //这个位置放置的是（运行本软件的）设备型号，本软件将根据这个设备型号去寻找相关资源比如图片等
     *     (char*)"Hello Giveda",//这个位置放置的是设备的显示名称，
     *     (char*)"36000",//这个位置放置的是设备端（即server端）所打开的TCP和UDP端口号，远程显示客户端会去连接TCP和UDP的这个端口号；UDP用于实现设备的搜索发现、TCP用于提供实际的远程显示服务；用户应当确保在此传入的端口号没有被其它软件占用。
     * };
     * argc = sizeof(param)/sizeof(char*);
     * 
     * GUIEvtLoop a( argc, param );//定义UI的事件循环
     * </pre>
     * 
     * @param argc 参数的数目
     * @param argv 参数列表
     */
    GUIEvtLoop( int argc, char** argv);
    
    /**
     * @brief 析构GUIEvtLoop，并设置全局变量uiEvtLp为0
     * 
     */
    virtual ~GUIEvtLoop();

    /**
     * @brief 进入事件循环，开始处理事件。除非 quit() 被调用，否则exec()将一直阻塞。如果你想退出exec()，请调用 quit() 。\n
     * 事件循环从窗口系统中接收事件并且把它们分派给应用程序的窗口部件。\n
     * 通常来说，在调用exec()之前，不会发生任何的用户交互。但 GCtrlMsgBox 这样的模式对话框是个例外，因为模式对话框拥有自己独立的 GCtrlMsgBox::exec() 。
     * 
     * @return int 此处返回的是你调用 quit() 时所传入的code参数；
     */
    int exec();
    
    /**
     * @brief 对事件循环进行递归。不要调用它，除非你是专家。
     * 
     * @return int 0代表成功，非0代表失败
     */
    static int enterLoop();
    
    /**
     * @brief 退出事件循环。不要调用它，除非你是专家。
     * 
     * @param level 指定事件循环的级别
     * @param code  事件循环的退出码
     * 
     * @return int 0代表成功，非0代表失败
     */
    static int exitLoop( int level, int code=0 );
    
    /**
     * @brief 返回当前的事件循环级别。不要调用它，除非你是专家。
     * 
     * @return int
     */
    int32_t loopLevel() const;
    
    /**
     * @brief 让程序退出；
     * 
     * @param code 指定程序的退出code，这个code将作为 exec() 的返回值
     * @return int 0代表成功，非0代表失败
     */
    static int quit( int code=0 );
    
#ifdef CONFIG_QT_SIMULATOR
    virtual bool event(GEvent* e);
#endif
    
#if defined(CONFIG_KEY_PRESS_EVENT_ENABLED)
    virtual void keyPressEvent ( GKeyEvent* );
#endif
    
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

public:
#if defined(CONFIG_V_SCREEN_ENABLED) || !defined(CONFIG_R_SCREEN_DISABLED)
    /**
     * @brief 返回目前的屏幕总数量，包括所有虚拟远程屏幕和真实硬件屏幕。
     * 
     * @return int8_t
     */
    int8_t screenCounts() const;
    
    /**
     * @brief 设置GUI窗口系统的宽高，以像素为单位。建议用户将GUI窗口系统的宽高设置为自己屏幕的分辨率。
     * 
     * @param w 宽
     * @param h 高
     * @return void
     */
    void setSize(T_OFFSET w, T_OFFSET h);
    
    GUIEvtLoopPriv* uiEvtLpPriv();
#endif

    /**
     * @brief 返回设备型号；详见 GUIEvtLoop( int argc, char** argv) ;
     * 
     * @return const GString&
     */
    const GString& deviceModel() const;
    
    /**
     * @brief 返回设备的显示名称；详见 GUIEvtLoop( int argc, char** argv) ;
     * 
     * @return const GString&
     */
    const GString& deviceTittle()  const;
    
#ifdef CONFIG_gImage
    /**
     * @brief 该函数用于设置软件所需要的图片数据；用户必须在构造GUIEvtLoop对象之前调用setImageDB。举例如下：\n
     * <pre>
     * #include <gUIEvtLoop.h>
     * #include "gImgApp.h" //这个头文件由Giveda框架自动生成，用户在需要使用时include进来即可
     * 
     * int main( int argc, char** argv )
     * {
     *     //在定义GUIEvtLoop对象之前，先执行setImageDB；
     *     //其中用到的imageDB(), GIMG_APP_NUM_MAX均在gImgApp.h中定义
     *     GUIEvtLoop::setImageDB( imageDB(), GIMG_APP_NUM_MAX);
     * 
     *     GUIEvtLoop a( argc, param );//定义UI的事件循环
     *     
     *     //用户在此进行自己需要的其它操作
     *     
     *     //进入事件循环
     *     return a.exec();
     * }
     * </pre>
     * @param p 直接传递imageDB()即可
     * @param nums 直接传递GIMG_APP_NUM_MAX即可
     * @return void
     */
    static void setImageDB(const GImageAttr* p, const uint16_t nums);
#endif
    
private:
    GUIEvtLoopPriv *appPriv;
};

///uiEvtLp全局变量指向GUIEvtLoop对象，用户可以在软件的任意地方通过uiEvtLp来访问GUIEvtLoop对象
DLL_PUBLIC extern GUIEvtLoop *uiEvtLp;


#endif  // GUIEVTLOOP_H

#endif  //CONFIG_gUIEvtLoop
