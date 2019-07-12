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
#include "gConstDefine.h"

#include <gUIEvtLoop.h>
#include <gCtrlButton.h>
#include <gCtrlItem.h>

GMyForm::GMyForm(GCtrlForm* parent, const char* name )
    :GCtrlForm( parent, name )
{
    m_pBtn1 = new GCtrlButton( "1", this, NULL, "btn1" );

    m_pBtn2 = new GCtrlButton( "2", this, NULL, "btn2" );
    m_pBtn2->moveBy( m_pBtn1->width(), 0 );

    m_pBtn3 = new GCtrlButton( "3", this, NULL, "btn3" );
    m_pBtn3->moveBy( 0, m_pBtn1->height() );

    m_pBtn4 = new GCtrlButton( "4", this, NULL, "btn4" );
    m_pBtn4->moveBy( m_pBtn1->width(), m_pBtn1->height() );
    
    m_txtInfo = new GCtrlText("按下空格键试试，上下左右方向键、回车、Esc都试试", this, NULL, "help");
    m_txtInfo->setColor( GColor(250, 250, 50) );
    m_txtInfo->moveBy(0, m_pBtn4->bottom() );

    connect( m_pBtn1, m_pBtn1->clicked, this, &GMyForm::slotBtn1BeenClicked );
    connect( m_pBtn2, m_pBtn2->clicked, this, &GMyForm::slotBtn2BeenClicked );
    connect( m_pBtn3, m_pBtn3->clicked, this, &GMyForm::slotBtn3BeenClicked );
    connect( m_pBtn4, m_pBtn4->clicked, this, &GMyForm::slotBtn4BeenClicked );
}

GMyForm::~GMyForm()
{
    if(m_pBtn1)
    {
        delete m_pBtn1;
        m_pBtn1 = NULL;
    }
    if(m_pBtn2)
    {
        delete m_pBtn2;
        m_pBtn2 = NULL;
    }
    if(m_pBtn3)
    {
        delete m_pBtn3;
        m_pBtn3 = NULL;
    }
    if(m_pBtn4)
    {
        delete m_pBtn4;
        m_pBtn4 = NULL;
    }
}

void GMyForm::slotBtn1BeenClicked()
{
    ZL_DEBUG("start\n");
    if(m_pBtn1->isVisible() )
    {
        m_pBtn1->hide();
    }
}

void GMyForm::slotBtn2BeenClicked()
{
    ZL_DEBUG("start\n");
    if(m_pBtn3->isVisible() )
    {
        m_pBtn3->hide();
    }
    else
    {
        m_pBtn3->show();
    }
}

void GMyForm::slotBtn3BeenClicked()
{
    ZL_DEBUG("start\n");
    if(m_pBtn4->isVisible() )
    {
        m_pBtn4->hide();
    }
    else
    {
        m_pBtn4->show();
    }
}

void GMyForm::slotBtn4BeenClicked()
{
    ZL_DEBUG("start\n");
    if(m_pBtn1->isVisible() )
    {
        m_pBtn1->hide();
    }
    else
    {
        m_pBtn1->show();
    }
}

bool GMyForm::keyPressEvent( GKeyEvent* e )
{
    ZL_DEBUG("start, className=[%s], name=[%s], key=[%d]\n", className(), name(), e->key() );
    bool bRet = true;
    switch(e->key() )
    {
    case Giveda::Key_Escape:
        ZL_DEBUG("now app will quit\n");
        uiEvtLp->quit();
        break;
    default:
        bRet = false;
    }

    return bRet;
}

// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
