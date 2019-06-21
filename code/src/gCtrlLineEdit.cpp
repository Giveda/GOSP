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

class GCtrlLineEditSelf
{
public:
    GCtrlLineEditSelf ( GCtrlForm* frm,  GCtrlLineEdit* le )
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
    : GMCtrlItem ( frm, parent, name ), leSpp ( new GCtrlLineEditSelf ( frm, this ) )
{
    frm->appendItem ( this );
    initAttributes();
}

GCtrlLineEdit::~GCtrlLineEdit()
{
    delete leSpp;
}

void GCtrlLineEdit::initAttributes()
{
    leSpp->m_strData = "";
    leSpp->m_nCursorPos = 0;
    leSpp->m_bIsShowCursor = false;
    leSpp->m_echoMode = Normal;
    leSpp->m_nKey_Backspace = Giveda::Key_Backspace;
    leSpp->m_txtDisplay.setTextFlags ( Giveda::AlignVCenter );

    connect ( &leSpp->m_timer, leSpp->m_timer.timeout, this, &GCtrlLineEdit::slotTimeOut );
    connect ( this, this->loseFocus, this, &GCtrlLineEdit::slotLoseFocus );
    connect ( this, this->getFocus, this, &GCtrlLineEdit::slotGetFocus );

    GCtrlDefaultAppStyle* pAppStyle = getDefaultAppStyle();
    GCtrlItemStyle* pStyle=NULL;
    while ( NULL== ( pStyle=pAppStyle->itemStyle ( className() ) ) )
    {
        pAppStyle->appendLineEditStyle();
    }
    leSpp->m_pixBg.setPixmap ( pStyle->pixmap ( leSpp->m_pixBg.name() ) );
    leSpp->m_txtDisplay.setFont ( pStyle->font ( leSpp->m_txtDisplay.name() ) );
    leSpp->m_txtDisplay.setColor ( pStyle->color ( leSpp->m_txtDisplay.name() ) );
    setSize ( leSpp->m_pixBg.width(), leSpp->m_pixBg.height() );
    leSpp->m_txtDisplay.setSize ( leSpp->m_pixBg.width(), leSpp->m_pixBg.height() );
}

void GCtrlLineEdit::slotTimeOut()
{
    leSpp->m_bIsShowCursor = !leSpp->m_bIsShowCursor;
    update();
}

void GCtrlLineEdit::slotGetFocus()
{
    leSpp->m_bIsShowCursor = true;
    leSpp->m_timer.start ( 1000 );
}

void GCtrlLineEdit::slotLoseFocus()
{
    leSpp->m_bIsShowCursor = false;
    leSpp->m_timer.stop();
}

void GCtrlLineEdit::paintEvent ( GPainter& p )
{
    leSpp->m_pixBg.draw ( p );

    GString str = leSpp->m_strData;
    if ( leSpp->m_echoMode == Password )
    {
        str.fill ( '*' );
    }
    if ( leSpp->m_bIsShowCursor )
    {
        str.insert ( leSpp->m_nCursorPos, "|" );
    }
    leSpp->m_txtDisplay.setText ( str );
    leSpp->m_txtDisplay.draw ( p );
}

bool GCtrlLineEdit::fwKeyPressEvent ( GKeyEvent* e )
{
    if ( leSpp->m_nKey_Backspace == e->key() )
    {
        leSpp->m_nCursorPos--;
        if ( leSpp->m_nCursorPos<0 )
        {
            leSpp->m_nCursorPos = 0;
        }
        else
        {
            leSpp->m_strData.remove ( leSpp->m_nCursorPos, 1 );
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
        leSpp->m_nCursorPos--;
        if ( leSpp->m_nCursorPos<0 )
        {
            leSpp->m_nCursorPos = 0;
        }
        update();
        return true;
    case Giveda::Key_Right:
        leSpp->m_nCursorPos++;
        if ( ( unsigned int ) leSpp->m_nCursorPos>leSpp->m_strData.length() )
        {
            leSpp->m_nCursorPos = leSpp->m_strData.length();
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

    leSpp->m_strData.insert ( leSpp->m_nCursorPos, s );
    leSpp->m_nCursorPos++;
    update();
    return true;
}
void GCtrlLineEdit::setEchoMode ( GCtrlLineEdit::EchoMode mode )
{
    leSpp->m_echoMode = mode;
}
GCtrlLineEdit::EchoMode GCtrlLineEdit::echoMode() const
{
    return leSpp->m_echoMode;
}
void GCtrlLineEdit::setTextGeometry ( int x, int y, int w, int h )
{
    leSpp->m_txtDisplay.setGeometry ( x, y, w, h );
}
void GCtrlLineEdit::setFont ( const GFont& font )
{
    leSpp->m_txtDisplay.setFont ( font );
}
void GCtrlLineEdit::setColor ( const GColor& color )
{
    leSpp->m_txtDisplay.setColor ( color );
}
void GCtrlLineEdit::setText ( const GString& str )
{
    leSpp->m_strData = str;
}
GString GCtrlLineEdit::text()
{
    return leSpp->m_strData;
}
void GCtrlLineEdit::clear()
{
    leSpp->m_strData = "";
}
void GCtrlLineEdit::setBackspaceKey ( int nKey )
{
    leSpp->m_nKey_Backspace = nKey;
}

// have a nice day ^_^
// have a nice day ^_^
