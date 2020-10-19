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

#include <gConstDefine.h>
#include <gUIEvtLoop.h>

#include <gCtrlButton.h>
#include <gCtrlItem.h>

GMyForm::GMyForm( GCtrlForm* parent, const char* name )
    :GCtrlForm(  parent, name )
{
    m_pBtn1 = new GCtrlButton( "Print", this, NULL, "btn1" );
    connect( m_pBtn1, m_pBtn1->clicked, this, &GMyForm::slotBtn1BeenClicked );

    m_pRadioBtnGroup = new GMyButtonGroup( "please select mode:", this, NULL, "radioButtonGroup" );
    m_pRadioBtnGroup->setGeometry( 0, m_pBtn1->height(),  220, 230);
    m_pRadioBtnGroup->getTitle()->setX(30);

    m_pTxt = new GCtrlText( "Mode1", this, NULL, "text1" );
    m_pTxt->moveBy( 0, m_pRadioBtnGroup->y() + m_pRadioBtnGroup->height());
    m_pTxt->setWidth( m_pTxt->width()*2 );
}

GMyForm::~GMyForm()
{
    if(m_pRadioBtnGroup)
    {
        delete m_pRadioBtnGroup;
        m_pRadioBtnGroup = NULL;
    }
    if(m_pBtn1)
    {
        delete m_pBtn1;
        m_pBtn1 = NULL;
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

void GMyForm::slotBtn1BeenClicked()
{
    if(m_pRadioBtnGroup->m_pRadioBtn1->isChecked() )
    {
        ZL_DEBUG("radio btn 1 is checked\n");
    }
    else if(m_pRadioBtnGroup->m_pRadioBtn2->isChecked() )
    {
        ZL_DEBUG("radio btn 2 is checked\n");
    }
    else if(m_pRadioBtnGroup->m_pRadioBtn3->isChecked() )
    {
        ZL_DEBUG("radio btn 3 is checked\n");
    }
    else if(m_pRadioBtnGroup->m_pRadioBtn4->isChecked() )
    {
        ZL_DEBUG("radio btn 4 is checked\n");
    }
    else
    {
        ZL_DEBUG("radio btn 5 is checked\n");
    }
}


GMyButtonGroup::GMyButtonGroup( const GString& title, GCtrlForm*frm, GMItem* parent, const char* name )
    :GCtrlRadioButtonGroup( title, frm, parent, name )
{
    m_pRadioBtn1 = new GCtrlRadioButton( "Mode1", frm, this, "radioButtonMode1", true);
    m_pRadioBtn1->setPosition(30, 30);

    int nTmp = 10;
    m_pRadioBtn2 = new GCtrlRadioButton( "Mode2", frm, this, "radioButtonMode2");
    m_pRadioBtn2->setPosition( m_pRadioBtn1->x(), m_pRadioBtn1->y()+m_pRadioBtn1->height()+nTmp );

    m_pRadioBtn3 = new GCtrlRadioButton( "Mode3", frm, this, "radioButtonMode3");
    m_pRadioBtn3->setPosition( m_pRadioBtn2->x(), m_pRadioBtn2->y()+m_pRadioBtn2->height()+nTmp );

    m_pRadioBtn4 = new GCtrlRadioButton( "Mode4", frm, this, "radioButtonMode4");
    m_pRadioBtn4->setPosition( m_pRadioBtn3->x(), m_pRadioBtn3->y()+m_pRadioBtn3->height()+nTmp );

    m_pRadioBtn5 = new GCtrlRadioButton( "Mode5", frm, this, "radioButtonMode5");
    m_pRadioBtn5->setPosition( m_pRadioBtn4->x(), m_pRadioBtn4->y()+m_pRadioBtn4->height()+nTmp );


    connect( m_pRadioBtn1, m_pRadioBtn1->checkedChanged, this, &GMyButtonGroup::slotRadioBtn1_checkedChanged );
    connect( m_pRadioBtn2, m_pRadioBtn2->checkedChanged, this, &GMyButtonGroup::slotRadioBtn2_checkedChanged );
    connect( m_pRadioBtn3, m_pRadioBtn3->checkedChanged, this, &GMyButtonGroup::slotRadioBtn3_checkedChanged );
    connect( m_pRadioBtn4, m_pRadioBtn4->checkedChanged, this, &GMyButtonGroup::slotRadioBtn4_checkedChanged );
    connect( m_pRadioBtn5, m_pRadioBtn5->checkedChanged, this, &GMyButtonGroup::slotRadioBtn5_checkedChanged );
}

GMyButtonGroup::~GMyButtonGroup()
{
#if 1
    if(m_pRadioBtn1)
    {
        delete m_pRadioBtn1;
        m_pRadioBtn1 = NULL;
    }
    if(m_pRadioBtn2)
    {
        delete m_pRadioBtn2;
        m_pRadioBtn2 = NULL;
    }
    if(m_pRadioBtn3)
    {
        delete m_pRadioBtn3;
        m_pRadioBtn3 = NULL;
    }
    if(m_pRadioBtn4)
    {
        delete m_pRadioBtn4;
        m_pRadioBtn4 = NULL;
    }
    if(m_pRadioBtn5)
    {
        delete m_pRadioBtn5;
        m_pRadioBtn5 = NULL;
    }
#endif
}

void GMyButtonGroup::slotRadioBtn1_checkedChanged()
{
    ZL_DEBUG("[%d]\n", m_pRadioBtn1->isChecked() );
    GMyForm* frm = (GMyForm*)form();
    if( m_pRadioBtn1->isChecked() )
    {
        frm->m_pTxt->setText("Mode1");
        frm->m_pTxt->update();
    }
    else
    {
    }
}

void GMyButtonGroup::slotRadioBtn2_checkedChanged()
{
    ZL_DEBUG("[%d]\n", m_pRadioBtn2->isChecked() );
    GMyForm* frm = (GMyForm*)form();
    if( m_pRadioBtn2->isChecked() )
    {
        frm->m_pTxt->setText("Mode2");
        frm->m_pTxt->update();
    }
    else
    {
    }
}

void GMyButtonGroup::slotRadioBtn3_checkedChanged()
{
    GMyForm* frm = (GMyForm*)form();
    if( m_pRadioBtn3->isChecked() )
    {
        frm->m_pTxt->setText("Mode3");
        frm->m_pTxt->update();
    }
    else
    {
    }
}

void GMyButtonGroup::slotRadioBtn4_checkedChanged()
{
    GMyForm* frm = (GMyForm*)form();
    if( m_pRadioBtn4->isChecked() )
    {
        frm->m_pTxt->setText("Mode4");
        frm->m_pTxt->update();
    }
    else
    {
    }
}

void GMyButtonGroup::slotRadioBtn5_checkedChanged()
{
    GMyForm* frm = (GMyForm*)form();
    if( m_pRadioBtn5->isChecked() )
    {
        frm->m_pTxt->setText("Mode5");
        frm->m_pTxt->update();
    }
    else
    {
    }
}

// nice day ^_^
