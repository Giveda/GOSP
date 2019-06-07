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

class GCtrlLineEditPrivate
{
public:
    GCtrlLineEditPrivate ( GCtrlForm* frm,  GCtrlLineEdit* le )
        :m_pixBg ( frm, le, "lineEditBg" ), m_txtDisplay ( frm, le, "lineEditText" ), m_timer ( le )
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
    : GMCtrlItem ( frm, parent, name ), lePriv ( new GCtrlLineEditPrivate ( frm, this ) )
{
    frm->appendItem ( this );
    initAttributes();
}

GCtrlLineEdit::~GCtrlLineEdit()
{
    delete lePriv;
}

void GCtrlLineEdit::initAttributes()
{
    lePriv->m_strData = "";
    lePriv->m_nCursorPos = 0;
    lePriv->m_bIsShowCursor = false;
    lePriv->m_echoMode = Normal;
    lePriv->m_nKey_Backspace = Giveda::Key_Backspace;
    lePriv->m_txtDisplay.setTextFlags ( Giveda::AlignVCenter );

    connect ( &lePriv->m_timer, lePriv->m_timer.timeout, this, &GCtrlLineEdit::slotTimeOut );
    connect ( this, this->loseFocus, this, &GCtrlLineEdit::slotLoseFocus );
    connect ( this, this->getFocus, this, &GCtrlLineEdit::slotGetFocus );

    GCtrlDefaultAppStyle* pAppStyle = getDefaultAppStyle();
    GCtrlItemStyle* pStyle=NULL;
    while ( NULL== ( pStyle=pAppStyle->itemStyle ( className() ) ) )
    {
        pAppStyle->appendLineEditStyle();
    }
    lePriv->m_pixBg.setPixmap ( pStyle->pixmap ( lePriv->m_pixBg.name() ) );
    lePriv->m_txtDisplay.setFont ( pStyle->font ( lePriv->m_txtDisplay.name() ) );
    lePriv->m_txtDisplay.setColor ( pStyle->color ( lePriv->m_txtDisplay.name() ) );
    setSize ( lePriv->m_pixBg.width(), lePriv->m_pixBg.height() );
    lePriv->m_txtDisplay.setSize ( lePriv->m_pixBg.width(), lePriv->m_pixBg.height() );
}

void GCtrlLineEdit::slotTimeOut()
{
    lePriv->m_bIsShowCursor = !lePriv->m_bIsShowCursor;
    update();
}

void GCtrlLineEdit::slotGetFocus()
{
    lePriv->m_bIsShowCursor = true;
    lePriv->m_timer.start ( 1000 );
}

void GCtrlLineEdit::slotLoseFocus()
{
    lePriv->m_bIsShowCursor = false;
    lePriv->m_timer.stop();
}

void GCtrlLineEdit::paintEvent ( GPainter& p )
{
    lePriv->m_pixBg.draw ( p );

    GString str = lePriv->m_strData;
    if ( lePriv->m_echoMode == Password )
    {
        str.fill ( '*' );
    }
    if ( lePriv->m_bIsShowCursor )
    {
        str.insert ( lePriv->m_nCursorPos, "|" );
    }
    lePriv->m_txtDisplay.setText ( str );
    lePriv->m_txtDisplay.draw ( p );
}

bool GCtrlLineEdit::fwKeyPressEvent ( GKeyEvent* e )
{
    if ( lePriv->m_nKey_Backspace == e->key() )
    {
        lePriv->m_nCursorPos--;
        if ( lePriv->m_nCursorPos<0 )
        {
            lePriv->m_nCursorPos = 0;
        }
        else
        {
            lePriv->m_strData.remove ( lePriv->m_nCursorPos, 1 );
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
        lePriv->m_nCursorPos--;
        if ( lePriv->m_nCursorPos<0 )
        {
            lePriv->m_nCursorPos = 0;
        }
        update();
        return true;
    case Giveda::Key_Right:
        lePriv->m_nCursorPos++;
        if ( ( unsigned int ) lePriv->m_nCursorPos>lePriv->m_strData.length() )
        {
            lePriv->m_nCursorPos = lePriv->m_strData.length();
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

    lePriv->m_strData.insert ( lePriv->m_nCursorPos, s );
    lePriv->m_nCursorPos++;
    update();
    return true;
}
void GCtrlLineEdit::setEchoMode ( GCtrlLineEdit::EchoMode mode )
{
    lePriv->m_echoMode = mode;
}
GCtrlLineEdit::EchoMode GCtrlLineEdit::echoMode() const
{
    return lePriv->m_echoMode;
}
void GCtrlLineEdit::setTextGeometry ( int x, int y, int w, int h )
{
    lePriv->m_txtDisplay.setGeometry ( x, y, w, h );
}
void GCtrlLineEdit::setFont ( const GFont& font )
{
    lePriv->m_txtDisplay.setFont ( font );
}
void GCtrlLineEdit::setColor ( const GColor& color )
{
    lePriv->m_txtDisplay.setColor ( color );
}
void GCtrlLineEdit::setText ( const GString& str )
{
    lePriv->m_strData = str;
}
GString GCtrlLineEdit::text()
{
    return lePriv->m_strData;
}
void GCtrlLineEdit::clear()
{
    lePriv->m_strData = "";
}
void GCtrlLineEdit::setBackspaceKey ( int nKey )
{
    lePriv->m_nKey_Backspace = nKey;
}
// have a nice day
// have a nice day
// have a nice day
