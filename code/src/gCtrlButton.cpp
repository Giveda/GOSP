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

class GMButtonFles
{
public:
    GMButtonFles( const GString& strCaption, const GPixmap& bg, const GPixmap& focus, GCtrlForm* form, GMItem* parent ) : m_txtCaption( strCaption, form, parent, "btnCaption" ),
        m_pixBg( bg, form, parent, "btnBg" ),
        m_pixFocus( focus, form, parent, "btnFocus" )
    {
        
    }
    
    GMButtonFles( GCtrlForm* form, GMItem* parent ) : m_txtCaption( form, parent, "btnCaption" ),
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
    btnFhl = new GMButtonFles( form, this);
    
    GCtrlDefaultAppStyle* pAppStyle = getDefaultAppStyle();
    GCtrlItemStyle* pStyle=NULL;
    
    while (NULL== (pStyle=pAppStyle->itemStyle("GCtrlButton" ) ) ) {
        pAppStyle->appendButtonStyle();
    }

    btnFhl->m_pixBg.setPixmap( pStyle->pixmap( btnFhl->m_pixBg.name() ) );
    btnFhl->m_pixFocus.setPixmap( pStyle->pixmap( btnFhl->m_pixFocus.name() ) );
    btnFhl->m_txtCaption.setFont( pStyle->font( btnFhl->m_txtCaption.name()) );
    btnFhl->m_txtCaption.setColor( pStyle->color( btnFhl->m_txtCaption.name() ) );
    int nW = btnFhl->m_pixFocus.width() > btnFhl->m_pixBg.width() ? btnFhl->m_pixFocus.width() : btnFhl->m_pixBg.width();
    int nH = btnFhl->m_pixFocus.height() > btnFhl->m_pixBg.height() ? btnFhl->m_pixFocus.height() : btnFhl->m_pixBg.height();
    setSize( nW, nH );
    btnFhl->m_txtCaption.setSize( nW, nH );
    btnFhl->m_txtCaption.setTextFlags( Giveda::WordBreak | Giveda::AlignCenter );
}

GMButton::GMButton( const GString& strCaption, const GPixmap& bg, const GPixmap& focus, GCtrlForm* form, GMItem* parent, const char* name )
    : GMCtrlItem( form, parent, name )
{
    btnFhl = new GMButtonFles(strCaption, bg, focus, form, this);
    
    int nW = btnFhl->m_pixFocus.width() > btnFhl->m_pixBg.width() ? btnFhl->m_pixFocus.width() : btnFhl->m_pixBg.width();
    int nH = btnFhl->m_pixFocus.height() > btnFhl->m_pixBg.height() ? btnFhl->m_pixFocus.height() : btnFhl->m_pixBg.height();
    setSize( nW, nH );
    btnFhl->m_txtCaption.setSize( nW, nH );
    btnFhl->m_txtCaption.setTextFlags( Giveda::WordBreak | Giveda::AlignCenter );
}

GMButton::~GMButton()
{
    delete btnFhl;
}

void GMButton::setCaption ( GString strCaption )
{
    btnFhl->m_txtCaption.setText ( strCaption );
}

void GMButton::setBgPixmap ( const GPixmap& pix )
{
    btnFhl->m_pixBg.setPixmap ( pix );
}

void GMButton::setFocusPixmap ( const GPixmap& pix )
{
    btnFhl->m_pixFocus.setPixmap ( pix );
}

void GMButton::paintEvent( GPainter& p )
{
    if(hasFocus())
    {
        btnFhl->m_pixFocus.draw(p);
    }
    else
    {
        btnFhl->m_pixBg.draw(p);
    }

    btnFhl->m_txtCaption.draw(p);
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
// have a nice day ^_^
// have a nice day ^_^
