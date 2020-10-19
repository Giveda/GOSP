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

#include "gCtrlLineEdit.h"
#include <gCtrlStyle.h>
#include <gConstDefine.h>
#include <gGlobal.h>

class GCtrlLineEditLMQ
{
public:
    GCtrlLineEditLMQ ( GCtrlForm* frm,  GCtrlLineEdit* le ):m_pixBg ( frm, le, "lineEditBg" ), m_txtDisplay ( frm, le, "lineEditText" ), m_timer ( le )
    {}
    
    GMPixmap m_pixBg;
    GMText m_txtDisplay;
    GTimer m_timer;
    GCtrlLineEdit::EchoMode m_echoMode;
    int m_nCursorPos;
    GString m_strData;
    bool m_bIsShowCursor;
    int m_nKey_Backspace;
};

GCtrlLineEdit::GCtrlLineEdit ( GCtrlForm* frm, GMItem* parent, const char* name )
    : GMCtrlItem ( frm, parent, name ), leHlo ( new GCtrlLineEditLMQ ( frm, this ) )
{
    frm->appendItem ( this );
    initAttributes();
}

GCtrlLineEdit::~GCtrlLineEdit()
{
    delete leHlo;
}

void GCtrlLineEdit::initAttributes()
{
    connect ( &leHlo->m_timer, leHlo->m_timer.timeout, this, &GCtrlLineEdit::slotTimeOut );
    connect ( this, this->loseFocus, this, &GCtrlLineEdit::slotLoseFocus );
    connect ( this, this->getFocus, this, &GCtrlLineEdit::slotGetFocus );
    
    leHlo->m_bIsShowCursor = false;
    leHlo->m_echoMode = Normal;
    leHlo->m_nKey_Backspace = Giveda::Key_Backspace;
    leHlo->m_txtDisplay.setTextFlags ( Giveda::AlignVCenter );
    leHlo->m_strData = "";
    leHlo->m_nCursorPos = 0;

    GCtrlDefaultAppStyle* pAppStyle = getDefaultAppStyle();
    GCtrlItemStyle* pStyle=NULL;
    while ( NULL== ( pStyle=pAppStyle->itemStyle ( className() ) ) )
    {
        pAppStyle->appendLineEditStyle();
    }
    leHlo->m_pixBg.setPixmap ( pStyle->pixmap ( leHlo->m_pixBg.name() ) );
    leHlo->m_txtDisplay.setFont ( pStyle->font ( leHlo->m_txtDisplay.name() ) );
    leHlo->m_txtDisplay.setColor ( pStyle->color ( leHlo->m_txtDisplay.name() ) );
    setSize ( leHlo->m_pixBg.width(), leHlo->m_pixBg.height() );
    leHlo->m_txtDisplay.setSize ( leHlo->m_pixBg.width(), leHlo->m_pixBg.height() );
}

void GCtrlLineEdit::slotTimeOut()
{
    leHlo->m_bIsShowCursor = !leHlo->m_bIsShowCursor;
    update();
}

void GCtrlLineEdit::slotGetFocus()
{
    leHlo->m_bIsShowCursor = true;
    leHlo->m_timer.start ( 1000 );
}

void GCtrlLineEdit::slotLoseFocus()
{
    leHlo->m_bIsShowCursor = false;
    leHlo->m_timer.stop();
}

void GCtrlLineEdit::paintEvent ( GPainter& p )
{
    leHlo->m_pixBg.draw ( p );

    GString str = leHlo->m_strData;
    if ( leHlo->m_echoMode == Password )
    {
        str.fill ( '*' );
    }
    if ( leHlo->m_bIsShowCursor )
    {
        str.insert ( leHlo->m_nCursorPos, "|" );
    }
    leHlo->m_txtDisplay.setText ( str );
    leHlo->m_txtDisplay.draw ( p );
}

bool GCtrlLineEdit::fwKeyPressEvent ( GKeyEvent* e )
{
    if ( leHlo->m_nKey_Backspace == e->key() )
    {
        leHlo->m_nCursorPos--;
        if ( leHlo->m_nCursorPos<0 )
        {
            leHlo->m_nCursorPos = 0;
        }
        else
        {
            leHlo->m_strData.remove ( leHlo->m_nCursorPos, 1 );
        }
        update();
        return true;
    }

    bool bRet = true;
    switch ( e->key() )
    {
    case Giveda::Key_Down:
    case Giveda::Key_Up:
        return false;
    case Giveda::Key_Left:
        leHlo->m_nCursorPos--;
        if ( leHlo->m_nCursorPos<0 )
        {
            leHlo->m_nCursorPos = 0;
        }
        update();
        return true;
    case Giveda::Key_Right:
        leHlo->m_nCursorPos++;
        if ( ( unsigned int ) leHlo->m_nCursorPos>leHlo->m_strData.length() )
        {
            leHlo->m_nCursorPos = leHlo->m_strData.length();
        }
        update();
        return true;
    default:
        bRet = false;
        break;
    }

    GString s = e->text();
    if ( s.isEmpty() || e->ascii() <32 )
    {
        return bRet;
    }

    leHlo->m_strData.insert ( leHlo->m_nCursorPos, s );
    leHlo->m_nCursorPos++;
    update();
    return true;
}
void GCtrlLineEdit::setEchoMode ( GCtrlLineEdit::EchoMode mode )
{
    leHlo->m_echoMode = mode;
}
GCtrlLineEdit::EchoMode GCtrlLineEdit::echoMode() const
{
    return leHlo->m_echoMode;
}
void GCtrlLineEdit::setTextGeometry ( int x, int y, int w, int h )
{
    leHlo->m_txtDisplay.setGeometry ( x, y, w, h );
}
void GCtrlLineEdit::setFont ( const GFont& font )
{
    leHlo->m_txtDisplay.setFont ( font );
}
void GCtrlLineEdit::setColor ( const GColor& color )
{
    leHlo->m_txtDisplay.setColor ( color );
}
void GCtrlLineEdit::setText ( const GString& str )
{
    leHlo->m_strData = str;
}

GString GCtrlLineEdit::text()
{
    return leHlo->m_strData;
}

void GCtrlLineEdit::clear()
{
    leHlo->m_strData = "";
}

void GCtrlLineEdit::setBackspaceKey ( int nKey )
{
    leHlo->m_nKey_Backspace = nKey;
}

// nice day ^_^
