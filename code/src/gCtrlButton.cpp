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

#include "gCtrlButton.h"
#include "gCtrlStyle.h"
#include "gCtrlForm.h"
#include <gConstDefine.h>
#include <gGlobal.h>

class GMButtonSelf
{
public:
    GMButtonSelf( const GString& strCaption, const GPixmap& bg, const GPixmap& focus, GCtrlForm* form, GMItem* parent ) : m_txtCaption( strCaption, form, parent, "btnCaption" ),
        m_pixBg( bg, form, parent, "btnBg" ),
        m_pixFocus( focus, form, parent, "btnFocus" )
    {
        
    }
    
    GMButtonSelf( GCtrlForm* form, GMItem* parent ) : m_txtCaption( form, parent, "btnCaption" ),
        m_pixBg( form, parent, "btnBg" ),
        m_pixFocus( form, parent, "btnFocus" )
    {}
    
    GMText m_txtCaption;
    GMPixmap m_pixBg;
    GMPixmap m_pixFocus;
};

GMButton::GMButton( GCtrlForm* form, GMItem* parent, const char* name )
    : GMCtrlItem( form, parent, name )
{
    btnSpp = new GMButtonSelf( form, this);
    
    GCtrlDefaultAppStyle* pAppStyle = getDefaultAppStyle();
    GCtrlItemStyle* pStyle=NULL;
    
    while (NULL== (pStyle=pAppStyle->itemStyle("GCtrlButton" ) ) ) {
        pAppStyle->appendButtonStyle();
    }

    btnSpp->m_pixBg.setPixmap( pStyle->pixmap( btnSpp->m_pixBg.name() ) );
    btnSpp->m_pixFocus.setPixmap( pStyle->pixmap( btnSpp->m_pixFocus.name() ) );
    btnSpp->m_txtCaption.setFont( pStyle->font( btnSpp->m_txtCaption.name()) );
    btnSpp->m_txtCaption.setColor( pStyle->color( btnSpp->m_txtCaption.name() ) );
    int nW = btnSpp->m_pixFocus.width() > btnSpp->m_pixBg.width() ? btnSpp->m_pixFocus.width() : btnSpp->m_pixBg.width();
    int nH = btnSpp->m_pixFocus.height() > btnSpp->m_pixBg.height() ? btnSpp->m_pixFocus.height() : btnSpp->m_pixBg.height();
    setSize( nW, nH );
    btnSpp->m_txtCaption.setSize( nW, nH );
    btnSpp->m_txtCaption.setTextFlags( Giveda::WordBreak | Giveda::AlignCenter );
}

GMButton::GMButton( const GString& strCaption, const GPixmap& bg, const GPixmap& focus, GCtrlForm* form, GMItem* parent, const char* name )
    : GMCtrlItem( form, parent, name )
{
    btnSpp = new GMButtonSelf(strCaption, bg, focus, form, this);
    
    int nW = btnSpp->m_pixFocus.width() > btnSpp->m_pixBg.width() ? btnSpp->m_pixFocus.width() : btnSpp->m_pixBg.width();
    int nH = btnSpp->m_pixFocus.height() > btnSpp->m_pixBg.height() ? btnSpp->m_pixFocus.height() : btnSpp->m_pixBg.height();
    setSize( nW, nH );
    btnSpp->m_txtCaption.setSize( nW, nH );
    btnSpp->m_txtCaption.setTextFlags( Giveda::WordBreak | Giveda::AlignCenter );
}

GMButton::~GMButton()
{
    delete btnSpp;
}

void GMButton::setCaption ( GString strCaption )
{
    btnSpp->m_txtCaption.setText ( strCaption );
}

void GMButton::setBgPixmap ( const GPixmap& pix )
{
    btnSpp->m_pixBg.setPixmap ( pix );
}

void GMButton::setFocusPixmap ( const GPixmap& pix )
{
    btnSpp->m_pixFocus.setPixmap ( pix );
}

void GMButton::paintEvent( GPainter& p )
{
    if(hasFocus())
    {
        btnSpp->m_pixFocus.draw(p);
    }
    else
    {
        btnSpp->m_pixBg.draw(p);
    }

    btnSpp->m_txtCaption.draw(p);
}

bool GMButton::fwKeyPressEvent( GKeyEvent* e )
{
    bool bRetVal = true;
    switch( e->key() )
    {
    case Giveda::Key_Return:
        clicked.emit();
        break;
    default:
        bRetVal = false;
        break;
    }

    return bRetVal;
}

GCtrlButton::GCtrlButton( GCtrlForm* form, GMItem* parent, const char* name )
    : GMButton( form, parent, name )
{
    form->appendItem(this);
}

GCtrlButton::GCtrlButton( const GString& strCaption, GCtrlForm* form, GMItem* parent, const char* name )
    : GMButton( form, parent, name )
{
    form->appendItem(this);
    setCaption( strCaption );
}

GCtrlButton::GCtrlButton( const GString& strCaption, const GPixmap& bg, const GPixmap& focus, GCtrlForm* form, GMItem* parent, const char* name )
    : GMButton( strCaption, bg, focus, form, parent, name )
{
    form->appendItem(this);
}

// a nice day ^_^
// a nice day ^_^
