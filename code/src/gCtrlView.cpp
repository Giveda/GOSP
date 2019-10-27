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

#include <gPainter.h>

#include "gConstDefine.h"
#include "gCtrlView.h"
#include "gCtrlForm.h"
#include "gCtrlStyle.h"
#include <gEvent.h>
#include <eventlist_p.h>
#include <gUIEvtLoop.h>
#include <gConstDefine.h>
#include <gGlobal.h>

#ifdef CONFIG_QT_SIMULATOR
#undef signals
#undef slots
#include <gscreenonqt.h>
#endif


int QFormList::compareItems ( GCtrlForm* p1, GCtrlForm* p2 )
{
    if ( p1->z() == p2->z() )
    {
        return 0;
    }
    
    else if ( p1->z() > p2->z() )
    {
        return 1;
    }
    
    else
    {
        return -1;
    }
}

GCtrlView::GCtrlView ( const GString& strPicPath )
{
    m_bgColor = GColor(0, 0, 0);
    m_pFocusForm = NULL;
    m_pAppStyle = new GCtrlDefaultAppStyle ( strPicPath );
    m_rect = GRect(0, 0, 1280, 720);
}

GCtrlView::~GCtrlView()
{
    if ( m_pAppStyle )
    {
        delete m_pAppStyle;
        m_pAppStyle = NULL;
    }
}

void GCtrlView::appendForm ( GCtrlForm* p )
{
    m_frmList.inSort ( p );
}

void GCtrlView::removeForm ( GCtrlForm* p )
{
    m_frmList.removeRef ( p );
}

void GCtrlView::setStyle ( GCtrlDefaultAppStyle* pStyle )
{
    m_pAppStyle = pStyle;
}

GCtrlDefaultAppStyle* GCtrlView::appStyle()
{
    return m_pAppStyle;
}

void GCtrlView::inSort ( GCtrlForm* pForm )
{
    m_frmList.removeRef ( pForm );
    m_frmList.inSort ( pForm );
}

void GCtrlView::fwKeyPress ( GKeyEvent* e )
{
    bool bRet = fwKeyPressEvent ( e );
    if ( true == bRet )
    {
        return;
    }

    uiEvtLp->keyPressEvent ( e );
}

void GCtrlView::keyReleaseEvent ( GKeyEvent*  )
{

}

bool GCtrlView::fwKeyPressEvent ( GKeyEvent* e )
{
    if ( !m_pFocusForm && !m_frmList.isEmpty() )
    {
        initFocus();
    }

    if ( !m_pFocusForm )
    {
        return false;
    }

    bool bRet = m_pFocusForm->fwKeyPress ( e );
    return bRet;
}

void GCtrlView::paintEvent ( GPaintEvent*  )
{
    if ( !m_pFocusForm && !m_frmList.isEmpty() )
    {
        initFocus();
    }

    static GPixmap paintBuffer ( width(), height() );
    GPainter p ( &paintBuffer );
    GPixmap* pBg = ( GPixmap* ) backgroundPixmap();
    if ( !pBg || pBg->isNull() )
    {
        p.fillRect ( rect(), GBrush ( backgroundColor() ) );
    }
    else
    {
        p.bitBlt ( &paintBuffer, GPoint(0, 0), pBg, rect(), CopyROP, true );
    }

    for ( GCtrlForm* pForm = m_frmList.first(); pForm!=NULL; pForm = m_frmList.next() )
    {
        if ( !pForm->isVisible() )
        {
            continue;
        }

        p.save();

        p.translate ( pForm->x(), pForm->y() );
        pForm->paintEvent ( p );

        p.restore();
    }

#ifdef CONFIG_R_SCREEN_LINUX_FB
    p.bitBlt ( g_screen, GPoint(0, 0), &paintBuffer, rect(), CopyROP, true );
#elif defined(CONFIG_QT_SIMULATOR)
    GScreenOnQt* w = GScreenOnQt::instance();
    w->vEnterLoopOnceWithImage(&paintBuffer);
    w->doRepaint( rect() );
#endif
}

void GCtrlView::setFocusToFrm ( GCtrlForm* pForm )
{
    if ( pForm == m_pFocusForm )
    {
        return ;
    }
    if ( m_pFocusForm )
    {
        m_pFocusForm->loseFocus();
        m_pFocusForm->update();
    }

    m_pFocusForm = pForm;

    if ( m_pFocusForm )
    {
        m_pFocusForm->getFocus();
        m_pFocusForm->update();
    }
}

void GCtrlView::initFocus()
{
    unsigned int nCount = m_frmList.count();
    int i=nCount-1;
    while ( 0 <= i )
    {
        if ( m_frmList.at ( i )->isVisible() && m_frmList.at ( i )->isFocusEnabled() )
        {
            m_pFocusForm = m_frmList.at ( i );
            m_pFocusForm->update();
            break;
        }

        i--;
    }
}

void GCtrlView::update ( GRect r )
{
    r = r.intersect( rect() );

    GEvent *evt = new GPaintEvent ( r );
    putEvt ( evt );
}

int GCtrlView::height() const
{
    return m_rect.height();
}

void GCtrlView::setSize ( int w, int h )
{
    m_rect.setSize( GSize(w, h) );
}

GRect GCtrlView::rect() const
{
    
    return m_rect;
}

int GCtrlView::width() const
{
    
    return m_rect.width();
    
}

// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
