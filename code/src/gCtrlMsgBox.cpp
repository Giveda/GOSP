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

#include <gCtrlStyle.h>
#include "gCtrlMsgBox.h"
#include "gCtrlButton.h"
#include "gCtrlView.h"
#include <gUIEvtLoop.h>
#include <gConstDefine.h>
#include <gGlobal.h>

GCtrlMsgBox::GCtrlMsgBox ( GString strTitle, GString strInfo, int nBtnNums,
                           GCtrlForm* parent, const char* name )
    : GCtrlForm ( parent, name ), 
    m_txtTitle ( strTitle, this, NULL, "msgBoxTitle" ), m_txtInfo ( strInfo, this, NULL, "msgBoxInfo" ), m_nBtnNums ( nBtnNums ), m_bIsInLoop ( false )
{
    GCtrlDefaultAppStyle* pAppStyle = getDefaultAppStyle();
    GCtrlItemStyle* pStyle=NULL;
    while ( NULL== ( pStyle=pAppStyle->itemStyle ( className() ) ) )
    {
        pAppStyle->appendMsgBoxStyle();
    }

    loadBackgroundPixmap ( pStyle->pixmap ( "msgBoxBg" ) );
    
    int nX=0, nY=0;
    if ( view()->width() > width() +2 )
        nX = ( view()->width() -width() ) /2;
    if ( view()->height() > height() +2 )
        nY = ( view()->height() -height() ) /2;
    setPosition ( nX, nY );
    m_txtTitle.setFont ( pStyle->font ( m_txtTitle.name() ) );
    m_txtTitle.setColor ( pStyle->color ( m_txtTitle.name() ) );
    m_txtInfo.setFont ( pStyle->font ( m_txtInfo.name() ) );
    m_txtInfo.setColor ( pStyle->color ( m_txtInfo.name() ) );
    
    GSize s = m_txtTitle.getSingleLineSize();
    m_txtTitle.setGeometry ( 0, 0, width(), s.height() );
    m_txtTitle.setTextFlags ( Giveda::AlignCenter | Giveda::WordBreak );
    int nX_info = width() /10;
    m_txtInfo.setGeometry ( nX_info, m_txtTitle.y() +m_txtTitle.height(), width()-2*nX_info, height() *2/3 );
    m_txtInfo.setTextFlags ( Giveda::AlignCenter | Giveda::WordBreak );
    
    GCtrlButton* pBtnOk = new GCtrlButton ( this, NULL, "msgBox Btn0" );
    connect ( pBtnOk, pBtnOk->clicked, this, &GCtrlMsgBox::slotBtnClicked );
    int nY_btns = m_txtInfo.y() +m_txtInfo.height();

    if ( m_nBtnNums == 1 )
    {
        pBtnOk->setPosition ( ( width()-pBtnOk->width() ) /2, nY_btns );
        m_btnList.append ( pBtnOk );
        return;
    }

    pBtnOk->setPosition ( width() *3/8-pBtnOk->width(), nY_btns );
    m_btnList.append ( pBtnOk );

    GCtrlButton* pBtnCancel = new GCtrlButton ( this, NULL, "msgBox Btn1" );
    connect ( pBtnCancel, pBtnCancel->clicked, this, &GCtrlMsgBox::slotBtnClicked );
    pBtnCancel->setPosition ( width() *5/8, nY_btns );
    m_btnList.append ( pBtnCancel );
}

GCtrlMsgBox::~GCtrlMsgBox()
{
    m_btnList.clear();
}

void GCtrlMsgBox::setBtnCaption ( GCtrlMsgBox::enumBtn btn, GString strCaption )
{
    m_btnList.at ( btn )->setCaption ( strCaption );
}

GCtrlMsgBox::enumBtn GCtrlMsgBox::exec()
{
    GCtrlForm* pBak = view()->getFocusFrm();
    show();
    m_btnList.at ( Btn_Ok )->setFocus();

    if ( !m_bIsInLoop )
    {
        m_bIsInLoop = true;
        uiEvtLp->enterLoop();
    }

    hide();
    
    view()->setFocusToFrm ( pBak );

    return ( enumBtn ) m_btnList.findRef ( ( GCtrlButton* ) getFocusItem() );
}

void GCtrlMsgBox::slotBtnClicked()
{
    if ( m_bIsInLoop )
    {
        uiEvtLp->exitLoop();
        m_bIsInLoop = false;
    }
}

// have a nice day ^_^
