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

class GMButtonMhL
{
public:
    GMButtonMhL( const GString& strCaption, const GPixmap& bg, const GPixmap& focus, GCtrlForm* form, GMItem* parent ) : m_txtCaption( strCaption, form, parent, "btnCaption" ),
        m_pixBg( bg, form, parent, "btnBg" ),
        m_pixFocus( focus, form, parent, "btnFocus" )
    {
        
    }
    
    GMButtonMhL( GCtrlForm* form, GMItem* parent ) : m_txtCaption( form, parent, "btnCaption" ),
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
    btnLqH = new GMButtonMhL( form, this);
    
    GCtrlDefaultAppStyle* pAppStyle = getDefaultAppStyle();
    GCtrlItemStyle* pStyle=NULL;
    
    while (NULL== (pStyle=pAppStyle->itemStyle("GCtrlButton" ) ) ) {
        pAppStyle->appendButtonStyle();
    }

    btnLqH->m_pixBg.setPixmap( pStyle->pixmap( btnLqH->m_pixBg.name() ) );
    btnLqH->m_pixFocus.setPixmap( pStyle->pixmap( btnLqH->m_pixFocus.name() ) );
    btnLqH->m_txtCaption.setFont( pStyle->font( btnLqH->m_txtCaption.name()) );
    btnLqH->m_txtCaption.setColor( pStyle->color( btnLqH->m_txtCaption.name() ) );
    int nW = btnLqH->m_pixFocus.width() > btnLqH->m_pixBg.width() ? btnLqH->m_pixFocus.width() : btnLqH->m_pixBg.width();
    int nH = btnLqH->m_pixFocus.height() > btnLqH->m_pixBg.height() ? btnLqH->m_pixFocus.height() : btnLqH->m_pixBg.height();
    setSize( nW, nH );
    btnLqH->m_txtCaption.setSize( nW, nH );
    btnLqH->m_txtCaption.setTextFlags( Giveda::WordBreak | Giveda::AlignCenter );
}

GMButton::GMButton( const GString& strCaption, const GPixmap& bg, const GPixmap& focus, GCtrlForm* form, GMItem* parent, const char* name )
    : GMCtrlItem( form, parent, name )
{
    btnLqH = new GMButtonMhL(strCaption, bg, focus, form, this);
    
    int nW = btnLqH->m_pixFocus.width() > btnLqH->m_pixBg.width() ? btnLqH->m_pixFocus.width() : btnLqH->m_pixBg.width();
    int nH = btnLqH->m_pixFocus.height() > btnLqH->m_pixBg.height() ? btnLqH->m_pixFocus.height() : btnLqH->m_pixBg.height();
    setSize( nW, nH );
    btnLqH->m_txtCaption.setSize( nW, nH );
    btnLqH->m_txtCaption.setTextFlags( Giveda::WordBreak | Giveda::AlignCenter );
}

GMButton::~GMButton()
{
    delete btnLqH;
}

void GMButton::setCaption ( GString strCaption )
{
    btnLqH->m_txtCaption.setText ( strCaption );
}

void GMButton::setBgPixmap ( const GPixmap& pix )
{
    btnLqH->m_pixBg.setPixmap ( pix );
}

void GMButton::setFocusPixmap ( const GPixmap& pix )
{
    btnLqH->m_pixFocus.setPixmap ( pix );
}

void GMButton::paintEvent( GPainter& p )
{
    if(hasFocus())
    {
        btnLqH->m_pixFocus.draw(p);
    }
    else
    {
        btnLqH->m_pixBg.draw(p);
    }

    btnLqH->m_txtCaption.draw(p);
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

// have a nice day ^_^
