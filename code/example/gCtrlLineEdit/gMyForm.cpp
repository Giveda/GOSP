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
#include <gCtrlLineEdit.h>

GMyForm::GMyForm( GCtrlForm* parent, const char* name )
    :GCtrlForm( parent, name )
{
    m_pBtn1 = new GCtrlButton( "mode", this, NULL, "btn1" );
    m_pBtn2 = new GCtrlButton( "print", this, NULL, "btn2" );

    connect( m_pBtn2, m_pBtn2->clicked, this, &GMyForm::slotBtn2BeenClicked);
    connect( m_pBtn1, m_pBtn1->clicked, this, &GMyForm::slotBtn1BeenClicked);

    m_pBtn2->moveBy( m_pBtn1->width(), 0 );

    m_pLineEdit = new GCtrlLineEdit( this, NULL, "lineEdit1");
    m_pLineEdit->moveBy( 0, m_pBtn1->height() );
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
}

void GMyForm::slotBtn1BeenClicked()
{
    ZL_DEBUG("start\n");
    if( GCtrlLineEdit::Normal == m_pLineEdit->echoMode() )
    {
        m_pLineEdit->setEchoMode( GCtrlLineEdit::Password );
    }
    else
    {
        m_pLineEdit->setEchoMode( GCtrlLineEdit::Normal );
    }
    m_pLineEdit->update();

}

void GMyForm::slotBtn2BeenClicked()
{
#if 0
    GString strTxt = m_pLineEdit->text();
    strTxt = strTxt.utf8();
    GString strDisplayTxt = m_pLineEdit->displayText();
    strDisplayTxt = strDisplayTxt.utf8();
    ZL_DEBUG("lineEdit1's text is [%s]\n", strTxt.utf8() );
    ZL_DEBUG("lineEdit1's displayText is [%s]\n", strDisplayTxt.utf8() );
#endif
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

// a nice day ^_^
