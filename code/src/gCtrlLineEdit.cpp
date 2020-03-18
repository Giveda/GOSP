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

class GCtrlLineEditHmq
{
public:
    GCtrlLineEditHmq ( GCtrlForm* frm,  GCtrlLineEdit* le ):m_pixBg ( frm, le, "lineEditBg" ), m_txtDisplay ( frm, le, "lineEditText" ), m_timer ( le )
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
    : GMCtrlItem ( frm, parent, name ), leMqs ( new GCtrlLineEditHmq ( frm, this ) )
{
    frm->appendItem ( this );
    initAttributes();
}

GCtrlLineEdit::~GCtrlLineEdit()
{
    delete leMqs;
}

void GCtrlLineEdit::initAttributes()
{
    connect ( &leMqs->m_timer, leMqs->m_timer.timeout, this, &GCtrlLineEdit::slotTimeOut );
    connect ( this, this->loseFocus, this, &GCtrlLineEdit::slotLoseFocus );
    connect ( this, this->getFocus, this, &GCtrlLineEdit::slotGetFocus );
    
    leMqs->m_bIsShowCursor = false;
    leMqs->m_echoMode = Normal;
    leMqs->m_nKey_Backspace = Giveda::Key_Backspace;
    leMqs->m_txtDisplay.setTextFlags ( Giveda::AlignVCenter );
    leMqs->m_strData = "";
    leMqs->m_nCursorPos = 0;

    GCtrlDefaultAppStyle* pAppStyle = getDefaultAppStyle();
    GCtrlItemStyle* pStyle=NULL;
    while ( NULL== ( pStyle=pAppStyle->itemStyle ( className() ) ) )
    {
        pAppStyle->appendLineEditStyle();
    }
    leMqs->m_pixBg.setPixmap ( pStyle->pixmap ( leMqs->m_pixBg.name() ) );
    leMqs->m_txtDisplay.setFont ( pStyle->font ( leMqs->m_txtDisplay.name() ) );
    leMqs->m_txtDisplay.setColor ( pStyle->color ( leMqs->m_txtDisplay.name() ) );
    setSize ( leMqs->m_pixBg.width(), leMqs->m_pixBg.height() );
    leMqs->m_txtDisplay.setSize ( leMqs->m_pixBg.width(), leMqs->m_pixBg.height() );
}

void GCtrlLineEdit::slotTimeOut()
{
    leMqs->m_bIsShowCursor = !leMqs->m_bIsShowCursor;
    update();
}

void GCtrlLineEdit::slotGetFocus()
{
    leMqs->m_bIsShowCursor = true;
    leMqs->m_timer.start ( 1000 );
}

void GCtrlLineEdit::slotLoseFocus()
{
    leMqs->m_bIsShowCursor = false;
    leMqs->m_timer.stop();
}

void GCtrlLineEdit::paintEvent ( GPainter& p )
{
    leMqs->m_pixBg.draw ( p );

    GString str = leMqs->m_strData;
    if ( leMqs->m_echoMode == Password )
    {
        str.fill ( '*' );
    }
    if ( leMqs->m_bIsShowCursor )
    {
        str.insert ( leMqs->m_nCursorPos, "|" );
    }
    leMqs->m_txtDisplay.setText ( str );
    leMqs->m_txtDisplay.draw ( p );
}

bool GCtrlLineEdit::fwKeyPressEvent ( GKeyEvent* e )
{
    if ( leMqs->m_nKey_Backspace == e->key() )
    {
        leMqs->m_nCursorPos--;
        if ( leMqs->m_nCursorPos<0 )
        {
            leMqs->m_nCursorPos = 0;
        }
        else
        {
            leMqs->m_strData.remove ( leMqs->m_nCursorPos, 1 );
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
        leMqs->m_nCursorPos--;
        if ( leMqs->m_nCursorPos<0 )
        {
            leMqs->m_nCursorPos = 0;
        }
        update();
        return true;
    case Giveda::Key_Right:
        leMqs->m_nCursorPos++;
        if ( ( unsigned int ) leMqs->m_nCursorPos>leMqs->m_strData.length() )
        {
            leMqs->m_nCursorPos = leMqs->m_strData.length();
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

    leMqs->m_strData.insert ( leMqs->m_nCursorPos, s );
    leMqs->m_nCursorPos++;
    update();
    return true;
}
void GCtrlLineEdit::setEchoMode ( GCtrlLineEdit::EchoMode mode )
{
    leMqs->m_echoMode = mode;
}
GCtrlLineEdit::EchoMode GCtrlLineEdit::echoMode() const
{
    return leMqs->m_echoMode;
}
void GCtrlLineEdit::setTextGeometry ( int x, int y, int w, int h )
{
    leMqs->m_txtDisplay.setGeometry ( x, y, w, h );
}
void GCtrlLineEdit::setFont ( const GFont& font )
{
    leMqs->m_txtDisplay.setFont ( font );
}
void GCtrlLineEdit::setColor ( const GColor& color )
{
    leMqs->m_txtDisplay.setColor ( color );
}
void GCtrlLineEdit::setText ( const GString& str )
{
    leMqs->m_strData = str;
}

GString GCtrlLineEdit::text()
{
    return leMqs->m_strData;
}

void GCtrlLineEdit::clear()
{
    leMqs->m_strData = "";
}

void GCtrlLineEdit::setBackspaceKey ( int nKey )
{
    leMqs->m_nKey_Backspace = nKey;
}

// reborn, a nice day ^_^
