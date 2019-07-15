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

class GCtrlLineEditMhL
{
public:
    GCtrlLineEditMhL ( GCtrlForm* frm,  GCtrlLineEdit* le ):m_pixBg ( frm, le, "lineEditBg" ), m_txtDisplay ( frm, le, "lineEditText" ), m_timer ( le )
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
    : GMCtrlItem ( frm, parent, name ), leLqH ( new GCtrlLineEditMhL ( frm, this ) )
{
    frm->appendItem ( this );
    initAttributes();
}

GCtrlLineEdit::~GCtrlLineEdit()
{
    delete leLqH;
}

void GCtrlLineEdit::initAttributes()
{
    connect ( &leLqH->m_timer, leLqH->m_timer.timeout, this, &GCtrlLineEdit::slotTimeOut );
    connect ( this, this->loseFocus, this, &GCtrlLineEdit::slotLoseFocus );
    connect ( this, this->getFocus, this, &GCtrlLineEdit::slotGetFocus );
    
    leLqH->m_bIsShowCursor = false;
    leLqH->m_echoMode = Normal;
    leLqH->m_nKey_Backspace = Giveda::Key_Backspace;
    leLqH->m_txtDisplay.setTextFlags ( Giveda::AlignVCenter );
    leLqH->m_strData = "";
    leLqH->m_nCursorPos = 0;

    GCtrlDefaultAppStyle* pAppStyle = getDefaultAppStyle();
    GCtrlItemStyle* pStyle=NULL;
    while ( NULL== ( pStyle=pAppStyle->itemStyle ( className() ) ) )
    {
        pAppStyle->appendLineEditStyle();
    }
    leLqH->m_pixBg.setPixmap ( pStyle->pixmap ( leLqH->m_pixBg.name() ) );
    leLqH->m_txtDisplay.setFont ( pStyle->font ( leLqH->m_txtDisplay.name() ) );
    leLqH->m_txtDisplay.setColor ( pStyle->color ( leLqH->m_txtDisplay.name() ) );
    setSize ( leLqH->m_pixBg.width(), leLqH->m_pixBg.height() );
    leLqH->m_txtDisplay.setSize ( leLqH->m_pixBg.width(), leLqH->m_pixBg.height() );
}

void GCtrlLineEdit::slotTimeOut()
{
    leLqH->m_bIsShowCursor = !leLqH->m_bIsShowCursor;
    update();
}

void GCtrlLineEdit::slotGetFocus()
{
    leLqH->m_bIsShowCursor = true;
    leLqH->m_timer.start ( 1000 );
}

void GCtrlLineEdit::slotLoseFocus()
{
    leLqH->m_bIsShowCursor = false;
    leLqH->m_timer.stop();
}

void GCtrlLineEdit::paintEvent ( GPainter& p )
{
    leLqH->m_pixBg.draw ( p );

    GString str = leLqH->m_strData;
    if ( leLqH->m_echoMode == Password )
    {
        str.fill ( '*' );
    }
    if ( leLqH->m_bIsShowCursor )
    {
        str.insert ( leLqH->m_nCursorPos, "|" );
    }
    leLqH->m_txtDisplay.setText ( str );
    leLqH->m_txtDisplay.draw ( p );
}

bool GCtrlLineEdit::fwKeyPressEvent ( GKeyEvent* e )
{
    if ( leLqH->m_nKey_Backspace == e->key() )
    {
        leLqH->m_nCursorPos--;
        if ( leLqH->m_nCursorPos<0 )
        {
            leLqH->m_nCursorPos = 0;
        }
        else
        {
            leLqH->m_strData.remove ( leLqH->m_nCursorPos, 1 );
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
        leLqH->m_nCursorPos--;
        if ( leLqH->m_nCursorPos<0 )
        {
            leLqH->m_nCursorPos = 0;
        }
        update();
        return true;
    case Giveda::Key_Right:
        leLqH->m_nCursorPos++;
        if ( ( unsigned int ) leLqH->m_nCursorPos>leLqH->m_strData.length() )
        {
            leLqH->m_nCursorPos = leLqH->m_strData.length();
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

    leLqH->m_strData.insert ( leLqH->m_nCursorPos, s );
    leLqH->m_nCursorPos++;
    update();
    return true;
}
void GCtrlLineEdit::setEchoMode ( GCtrlLineEdit::EchoMode mode )
{
    leLqH->m_echoMode = mode;
}
GCtrlLineEdit::EchoMode GCtrlLineEdit::echoMode() const
{
    return leLqH->m_echoMode;
}
void GCtrlLineEdit::setTextGeometry ( int x, int y, int w, int h )
{
    leLqH->m_txtDisplay.setGeometry ( x, y, w, h );
}
void GCtrlLineEdit::setFont ( const GFont& font )
{
    leLqH->m_txtDisplay.setFont ( font );
}
void GCtrlLineEdit::setColor ( const GColor& color )
{
    leLqH->m_txtDisplay.setColor ( color );
}
void GCtrlLineEdit::setText ( const GString& str )
{
    leLqH->m_strData = str;
}

GString GCtrlLineEdit::text()
{
    return leLqH->m_strData;
}

void GCtrlLineEdit::clear()
{
    leLqH->m_strData = "";
}

void GCtrlLineEdit::setBackspaceKey ( int nKey )
{
    leLqH->m_nKey_Backspace = nKey;
}

// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
