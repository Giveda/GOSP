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

#include <gUIEvtLoop.h>
#include <gConstDefine.h>
#include <gCtrlButton.h>

GMyForm::GMyForm( GCtrlForm* parent, const char* name )
    :GCtrlForm(parent, name ), m_progressBar( this, NULL, "progressBar")
{
    m_pBtn1 = new GCtrlButton( "+", this, NULL, "btn1" );
    m_pBtn2 = new GCtrlButton( "-", this, NULL, "btn2" );

    connect( m_pBtn2, m_pBtn2->clicked, this, &GMyForm::slotBtn2BeenClicked);
    connect( m_pBtn1, m_pBtn1->clicked, this, &GMyForm::slotBtn1BeenClicked);

    m_pBtn2->moveBy( m_pBtn1->width(), 0 );

    m_progressBar.moveBy( 100, m_pBtn1->height() );
    m_progressBar.slotSetTotalSteps( 100 );
    m_progressBar.slotSetStepLength( 5 );
    m_progressBar.setProgressBgHeight( 20 );
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
    m_progressBar.slotSetProgress( m_progressBar.progress() + m_progressBar.stepLength() );
}

void GMyForm::slotBtn2BeenClicked()
{
    ZL_DEBUG("start\n");
    m_progressBar.slotSetProgress( m_progressBar.progress() - m_progressBar.stepLength() );
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
