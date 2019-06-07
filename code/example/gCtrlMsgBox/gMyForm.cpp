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

#include "gMyForm.h"

#include <gCtrlButton.h>
#include <gConstDefine.h>

#include <gUIEvtLoop.h>

GMyMsgBox::GMyMsgBox ( GString strTitle, GString strInfo, int nBtnNums,
                       GCtrlForm* parent, const char* name )
    :GCtrlMsgBox ( strTitle, strInfo, nBtnNums, parent, name )
{
    setZ ( 3 );
    m_txtTitle.setGeometry ( 0, 0, width(), 59 );
}


GMyForm::GMyForm ( GCtrlForm* parent, const char* name )
    :GCtrlForm ( parent, name )
{
    m_pBtn1 = new GCtrlButton ( "开始", this, NULL, "btn1" );
    connect ( m_pBtn1, m_pBtn1->clicked, this, &GMyForm::slotBtn1BeenClicked );
}

GMyForm::~GMyForm()
{
    if ( m_pBtn1 )
    {
        delete m_pBtn1;
        m_pBtn1 = NULL;
    }
}

void GMyForm::slotBtn1BeenClicked()
{
    ZL_DEBUG ( "start\n" );
    GMyMsgBox box ( GString ( "提示框" ), GString ( "你是否看见弹出的提示框？" ), 2, this, "msgBox" );
    box.setBtnCaption ( GCtrlMsgBox::Btn_Ok, GString ( "确定" ) );
    box.setBtnCaption ( GCtrlMsgBox::Btn_Cancel, GString ( "取消" ) );
    switch ( box.exec() )
    {
    case GCtrlMsgBox::Btn_Ok:
        ZL_DEBUG ( "ok has been clicked\n" );
        break;
    case GCtrlMsgBox::Btn_Cancel:
        ZL_DEBUG ( "cancel has been clicked\n" );
        break;
    default:
        break;
    }
}

bool GMyForm::keyPressEvent ( GKeyEvent* e )
{
    ZL_DEBUG ( "start, className=[%s], name=[%s], key=[%d]\n", className(), name(), e->key() );
    bool bRet = true;
    switch ( e->key() )
    {
    case Giveda::Key_Escape:
        ZL_DEBUG ( "now app will quit\n" );
        uiEvtLp->quit();
        break;
    default:
        bRet = false;
    }

    return bRet;
}
// have a nice day
// have a nice day
// have a nice day
