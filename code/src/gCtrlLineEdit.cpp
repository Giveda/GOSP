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

class GCtrlLineEditFles
{
public:
    GCtrlLineEditFles ( GCtrlForm* frm,  GCtrlLineEdit* le ):m_pixBg ( frm, le, "lineEditBg" ), m_txtDisplay ( frm, le, "lineEditText" ), m_timer ( le )
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
    : GMCtrlItem ( frm, parent, name ), leFhl ( new GCtrlLineEditFles ( frm, this ) )
{
    frm->appendItem ( this );
    initAttributes();
}

GCtrlLineEdit::~GCtrlLineEdit()
{
    delete leFhl;
}

void GCtrlLineEdit::initAttributes()
{
    leFhl->m_strData = "";
    leFhl->m_nCursorPos = 0;
    leFhl->m_bIsShowCursor = false;
    leFhl->m_echoMode = Normal;
    leFhl->m_nKey_Backspace = Giveda::Key_Backspace;
    leFhl->m_txtDisplay.setTextFlags ( Giveda::AlignVCenter );

    connect ( &leFhl->m_timer, leFhl->m_timer.timeout, this, &GCtrlLineEdit::slotTimeOut );
    connect ( this, this->loseFocus, this, &GCtrlLineEdit::slotLoseFocus );
    connect ( this, this->getFocus, this, &GCtrlLineEdit::slotGetFocus );

    GCtrlDefaultAppStyle* pAppStyle = getDefaultAppStyle();
    GCtrlItemStyle* pStyle=NULL;
    while ( NULL== ( pStyle=pAppStyle->itemStyle ( className() ) ) )
    {
        pAppStyle->appendLineEditStyle();
    }
    leFhl->m_pixBg.setPixmap ( pStyle->pixmap ( leFhl->m_pixBg.name() ) );
    leFhl->m_txtDisplay.setFont ( pStyle->font ( leFhl->m_txtDisplay.name() ) );
    leFhl->m_txtDisplay.setColor ( pStyle->color ( leFhl->m_txtDisplay.name() ) );
    setSize ( leFhl->m_pixBg.width(), leFhl->m_pixBg.height() );
    leFhl->m_txtDisplay.setSize ( leFhl->m_pixBg.width(), leFhl->m_pixBg.height() );
}

void GCtrlLineEdit::slotTimeOut()
{
    leFhl->m_bIsShowCursor = !leFhl->m_bIsShowCursor;
    update();
}

void GCtrlLineEdit::slotGetFocus()
{
    leFhl->m_bIsShowCursor = true;
    leFhl->m_timer.start ( 1000 );
}

void GCtrlLineEdit::slotLoseFocus()
{
    leFhl->m_bIsShowCursor = false;
    leFhl->m_timer.stop();
}

void GCtrlLineEdit::paintEvent ( GPainter& p )
{
    leFhl->m_pixBg.draw ( p );

    GString str = leFhl->m_strData;
    if ( leFhl->m_echoMode == Password )
    {
        str.fill ( '*' );
    }
    if ( leFhl->m_bIsShowCursor )
    {
        str.insert ( leFhl->m_nCursorPos, "|" );
    }
    leFhl->m_txtDisplay.setText ( str );
    leFhl->m_txtDisplay.draw ( p );
}

bool GCtrlLineEdit::fwKeyPressEvent ( GKeyEvent* e )
{
    if ( leFhl->m_nKey_Backspace == e->key() )
    {
        leFhl->m_nCursorPos--;
        if ( leFhl->m_nCursorPos<0 )
        {
            leFhl->m_nCursorPos = 0;
        }
        else
        {
            leFhl->m_strData.remove ( leFhl->m_nCursorPos, 1 );
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
        leFhl->m_nCursorPos--;
        if ( leFhl->m_nCursorPos<0 )
        {
            leFhl->m_nCursorPos = 0;
        }
        update();
        return true;
    case Giveda::Key_Right:
        leFhl->m_nCursorPos++;
        if ( ( unsigned int ) leFhl->m_nCursorPos>leFhl->m_strData.length() )
        {
            leFhl->m_nCursorPos = leFhl->m_strData.length();
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

    leFhl->m_strData.insert ( leFhl->m_nCursorPos, s );
    leFhl->m_nCursorPos++;
    update();
    return true;
}
void GCtrlLineEdit::setEchoMode ( GCtrlLineEdit::EchoMode mode )
{
    leFhl->m_echoMode = mode;
}
GCtrlLineEdit::EchoMode GCtrlLineEdit::echoMode() const
{
    return leFhl->m_echoMode;
}
void GCtrlLineEdit::setTextGeometry ( int x, int y, int w, int h )
{
    leFhl->m_txtDisplay.setGeometry ( x, y, w, h );
}
void GCtrlLineEdit::setFont ( const GFont& font )
{
    leFhl->m_txtDisplay.setFont ( font );
}
void GCtrlLineEdit::setColor ( const GColor& color )
{
    leFhl->m_txtDisplay.setColor ( color );
}
void GCtrlLineEdit::setText ( const GString& str )
{
    leFhl->m_strData = str;
}

GString GCtrlLineEdit::text()
{
    return leFhl->m_strData;
}

void GCtrlLineEdit::clear()
{
    leFhl->m_strData = "";
}

void GCtrlLineEdit::setBackspaceKey ( int nKey )
{
    leFhl->m_nKey_Backspace = nKey;
}

// have a nice day ^_^
// have a nice day ^_^
