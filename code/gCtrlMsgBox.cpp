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

#ifdef CONFIG_gCtrlMsgBox

#ifndef GCTRLMSGBOX_H
#define GCTRLMSGBOX_H

#include <gCtrlForm.h>
#include <gCtrlItem.h>
#include <gPtrList.h>

class GCtrlButton;
class GCtrlMsgBoxPrivate;

/*! @file  gCtrlMsgBox.h
 * @brief  GCtrlMsgBox 对话框。
 */

/*!
 * @class GCtrlMsgBox
 * @brief 对话框
 * 
 * @author 明心
 * @version 1.0.0
 * @date 2019-2-4
 */
class DLL_PUBLIC GCtrlMsgBox: public GCtrlForm
{
    SET_CLASS_NAME( GCtrlMsgBox )
    G_DISABLE_ASSIGN ( GCtrlMsgBox )
public:
    enum enumBtn ///这个enum用于索引对话框中的按钮。如果需要更多的按钮，请直接使用从2开始的数字来索引你自定义的按钮。
    {
         ///确定按钮，值为0
        Btn_Ok=0, 
        ///取消按钮，值为1
        Btn_Cancel=1,
    };
    
    /**
     * @brief 构造一个对话框
     * 
     * @param strTitle 标题
     * @param strInfo 内容
     * @param nBtnNums 按钮数量
     * @param parent ...
     * @param name ...
     */
    GCtrlMsgBox ( const GString& strTitle, const GString& strInfo, int nBtnNums, GCtrlForm* parent = 0, const char* name = 0 );
    
    virtual ~GCtrlMsgBox();
    
    /**
     * @brief 设置按钮的文字标题
     * 
     * @param btn enumBtn 
     * @param strCaption 标题
     * @return void
     */
    void setBtnCaption ( enumBtn btn, GString strCaption );
    
    /**
     * @brief 启动对话框的事件循环；除非用户通过点击按钮来关闭对话框，否则本函数将永远阻塞。
     * 
     * @return GCtrlMsgBox::enumBtn 代表被用户点击的那个按钮
     */
    enumBtn exec();
    
#ifndef CONFIG_STD_CPP11
private slots:
    DLL_LOCAL static void slotBtnClicked(GObject*);
#endif
private slots:
    DLL_LOCAL void slotBtnClicked();

protected:
    GCtrlText m_txtTitle;
    GCtrlText m_txtInfo;
    unsigned int m_nBtnNums;
    GPtrList<GCtrlButton> m_btnList;
    
private:
    GCtrlMsgBoxPrivate *mbPriv;
};

#endif

#endif  //CONFIG_gCtrlMsgBox
