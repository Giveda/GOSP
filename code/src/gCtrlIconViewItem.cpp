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

#include "gCtrlIconViewItem.h"
#include "gCtrlIconView.h"
#include <gConstDefine.h>
#include <gGlobal.h>

GCtrlIconViewItem::GCtrlIconViewItem ( const GImage& img, const GString& str,  GCtrlForm* frm, GCtrlIconView* pIconView, const char* name )
    :GMCtrlItem ( frm, pIconView, name ), m_txtTitle ( str, frm, this, "iconViewItemTitle" ), m_imgIcon ( img, frm, this, "iconViewItemIcon" )
{
    m_txtTitle.setVisible ( false );
}

GCtrlIconViewItem::~GCtrlIconViewItem()
{
#if 0
    if ( m_pIcon )
    {
        delete m_pIcon;
    }
#endif
}

void GCtrlIconViewItem::paintEvent ( GPainter& p )
{
    m_imgIcon.draw ( p );

    if ( m_txtTitle.isVisible() )
    {
        m_txtTitle.draw ( p );
    }
}

void GCtrlIconViewItem::setGeometry ( int x, int y, int w, int h )
{
    GMItem::setGeometry ( x,y,w,h );
    int nTmp = width() - m_imgIcon.width();
    if ( nTmp > 1 )
    {
        m_imgIcon.setX ( nTmp/2 );
    }
    m_txtTitle.setGeometry ( 0, m_imgIcon.height(), width(), height()-m_imgIcon.height() );
}

bool GCtrlIconViewItem::fwKeyPressEvent ( GKeyEvent* e )
{
    bool bRetVal = true;
    switch ( e->key() )
    {
    case Giveda::Key_Return:
        {
            GCtrlIconView* pBox = ( GCtrlIconView* ) parent();
            pBox->emitSelected ( this );
        }
        break;
    default:
        bRetVal = false;
        break;
    }

    return bRetVal;
}
