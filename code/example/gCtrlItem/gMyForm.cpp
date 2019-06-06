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

GMyForm::GMyForm(GCtrlForm* parent, const char* name )
    :GCtrlForm( parent, name ), m_txtImg( this, NULL, "label for image1"), m_img( this, NULL, "image1"), m_txtPixmap( this, NULL, "label for pixmap1"), m_pix( this, NULL, "pixmap1")
{
    m_txtImg.setText( GString("这是一个Image，可以对其进行缩放") );
    m_txtImg.setColor( GColor(0, 0, 0) );
    GSize s = m_txtImg.getSingleLineSize();
    m_txtImg.setSize( s.width()/2+20, s.height()*2 );
    m_txtImg.moveBy( 0, 50 );
    m_img.load("./pics/pacman.png");
    m_img.setSize( m_txtImg.width(), m_img.height()/2 );
    m_img.moveBy( 0, m_txtImg.y()+m_txtImg.height() );

    m_txtPixmap.setText( GString("这是一个pixmap，没有缩放功能") );
    m_txtPixmap.setColor( GColor(0, 0, 0) );
    s = m_txtImg.getSingleLineSize();
    m_txtPixmap.setSize( s.width()/2, s.height()*2);
    m_txtPixmap.moveBy( m_txtImg.x()+m_txtImg.width()+30, m_txtImg.y() );
    m_pix.load("./pics/tree.png");
    m_pix.setSize( m_pix.width(), m_pix.height() );
    m_pix.moveBy( m_txtPixmap.x(), m_txtPixmap.y()+m_txtPixmap.height() );
}

GMyForm::~GMyForm()
{
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

// have a nice day
// have a nice day
