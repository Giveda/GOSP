/*
 * Copyright (C) 2019  明心  <imleizhang@qq.com>
 * All rights reserved.
 *
 * This program is an open-source software; and it is distributed in the hope
 * that it will be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.
 * This program is not a free software; so you can not redistribute it(include
 * binary form and source code form) without my authorization. And if you
 * need use it for any commercial purpose, you should first get commercial
 * authorization from me, otherwise there will be your's legal&credit risks.
 *
 */

#ifndef GCTRLMSGBOX_H
#define GCTRLMSGBOX_H

#include "gCtrlForm.h"
#include "gCtrlItem.h"
#include <gConstDefine.h>
#include <gGlobal.h>

class GCtrlButton;

class GCtrlMsgBox: public GCtrlForm
{
    SET_CLASS_NAME(GCtrlMsgBox)
    G_DISABLE_ASSIGN(GCtrlMsgBox)

public:
    enum enumBtn {Btn_Ok=0, Btn_Cancel=1};
    GCtrlMsgBox( GString strTitle, GString strInfo, int nBtnNums, GCtrlForm* parent = 0, const char* name = 0);
    ~GCtrlMsgBox();
    void setBtnCaption( enumBtn btn, GString strCaption );
    enumBtn exec();
    
private slots:
    void slotBtnClicked();
    
protected:
    GCtrlText m_txtTitle;
    GCtrlText m_txtInfo;
    unsigned int m_nBtnNums;
    GPtrList<GCtrlButton> m_btnList;
    
private:
    bool m_bIsInLoop;
};

#endif


// have a nice day ^_^
// have a nice day ^_^
