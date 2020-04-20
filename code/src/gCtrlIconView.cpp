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

#include "gCtrlIconView.h"
#include <gCtrlStyle.h>
#include <gConstDefine.h>
#include <gGlobal.h>

class GCtrlIconViewLMQ
{
public:
    GCtrlIconViewLMQ( GCtrlForm* frm, GMItem* parent, GCtrlIconView *ikv )
    : m_imgFocusIn ( frm, parent, "iconViewFocusIn" ), m_imgFocusOut ( frm, parent, "iconViewFocusOut" ), m_timerForShowItemInfo ( ikv )
    {}
    
    GPtrList<GCtrlIconViewItem> m_itemList;
    GMImage m_imgFocusIn;
    GMImage m_imgFocusOut;
    GTimer m_timerForShowItemInfo;
    unsigned int m_nCurItemIndex;
    int m_nColumnNums;
    int m_nRowNums;
    bool m_bIsNeedDoLayout;
    bool m_bIsNeedShowItemInfo;
    bool m_bSendHighLighted;
    unsigned int m_nDrawFrom;
    unsigned int m_nDrawTo;
    unsigned int m_nMaxWofItem;
    unsigned int m_nMaxHofItem;
};

GCtrlIconView::GCtrlIconView ( GCtrlForm* frm, GMItem* parent, const char* name )
    : GMContainerItem ( frm, parent, name )
{
    ikvHlo = new GCtrlIconViewLMQ(frm, parent, this);
    frm->appendItem ( this );
    ikvHlo->m_nDrawFrom = 0;
    ikvHlo->m_nRowNums = 3;
    setColumnNums ( 3 );
    setRowNums ( 3 );

    connect ( &ikvHlo->m_timerForShowItemInfo,
              ikvHlo->m_timerForShowItemInfo.timeout, this, &GCtrlIconView::slotFocusChangedTo );
    connect ( this, this->loseFocus, this, &GCtrlIconView::slotLoseFocus );
    connect ( this, this->getFocus, this, &GCtrlIconView::slotGetFocus );

    ikvHlo->m_nCurItemIndex = 0;
    ikvHlo->m_bIsNeedDoLayout = true;
    ikvHlo->m_bIsNeedShowItemInfo = true;
    ikvHlo->m_bSendHighLighted = false;

    GCtrlDefaultAppStyle* pAppStyle = getDefaultAppStyle();
    GCtrlItemStyle* pStyle=NULL;
    while ( NULL== ( pStyle=pAppStyle->itemStyle ( className() ) ) )
    {
        pAppStyle->appendIconViewStyle();
    }

    ikvHlo->m_imgFocusIn.setImage ( pStyle->pixmap ( ikvHlo->m_imgFocusIn.name() ) );
    ikvHlo->m_imgFocusOut.setImage ( pStyle->pixmap ( ikvHlo->m_imgFocusOut.name() ) );
}

GCtrlIconView::~GCtrlIconView()
{
    ikvHlo->m_itemList.clear();
    delete ikvHlo;
}

void GCtrlIconView::paintEvent ( GPainter& p )
{
    if ( ikvHlo->m_itemList.isEmpty() )
    {
        return;
    }

    int nDrawTo = ikvHlo->m_itemList.count()-1<ikvHlo->m_nDrawTo ? ikvHlo->m_itemList.count()-1 : ikvHlo->m_nDrawTo;
    GCtrlIconViewItem* pItem=NULL;
    if ( ikvHlo->m_bIsNeedDoLayout )
    {
        ikvHlo->m_itemList.at ( ikvHlo->m_nDrawFrom )->setGeometry ( 0, 0, ikvHlo->m_nMaxWofItem, ikvHlo->m_nMaxHofItem );
        int nRightEdge = ikvHlo->m_nMaxWofItem* ( ikvHlo->m_nColumnNums-1 );
        for ( int j=ikvHlo->m_nDrawFrom; j<nDrawTo; j++ )
        {
            pItem = ikvHlo->m_itemList.at ( j+1 );
            if ( ikvHlo->m_itemList.at ( j )->x() == nRightEdge )
            {
                pItem->setGeometry ( 0, ikvHlo->m_itemList.at ( j )->y() +ikvHlo->m_nMaxHofItem, ikvHlo->m_nMaxWofItem, ikvHlo->m_nMaxHofItem );
            }
            else
            {
                pItem->setGeometry ( ikvHlo->m_itemList.at ( j )->x() +ikvHlo->m_nMaxWofItem, ikvHlo->m_itemList.at ( j )->y(), ikvHlo->m_nMaxWofItem, ikvHlo->m_nMaxHofItem );
            }
        }

        ikvHlo->m_bIsNeedDoLayout = false;
    }

    ikvHlo->m_itemList.at ( ikvHlo->m_nCurItemIndex )->getTxt()->setVisible ( true );

    for ( int j=ikvHlo->m_nDrawFrom; j<=nDrawTo; j++ )
    {
        pItem = ikvHlo->m_itemList.at ( j );

        p.save();
        p.translate ( pItem->x(), pItem->y() );
        pItem->draw ( p );
        p.restore();
    }

    GMImage* pIcon=ikvHlo->m_itemList.at ( ikvHlo->m_nCurItemIndex )->getIcon();
    if ( hasFocus() )
    {
        ikvHlo->m_imgFocusIn.setGeometry ( ikvHlo->m_itemList.at ( ikvHlo->m_nCurItemIndex )->x(), ikvHlo->m_itemList.at ( ikvHlo->m_nCurItemIndex )->y(), ikvHlo->m_nMaxWofItem, pIcon->height() );
        ikvHlo->m_imgFocusIn.draw ( p );
    }
    else
    {
        ikvHlo->m_imgFocusOut.setGeometry ( ikvHlo->m_itemList.at ( ikvHlo->m_nCurItemIndex )->x(), ikvHlo->m_itemList.at ( ikvHlo->m_nCurItemIndex )->y(), ikvHlo->m_nMaxWofItem, pIcon->height() );
        ikvHlo->m_imgFocusOut.draw ( p );
    }

    if ( ikvHlo->m_bSendHighLighted )
    {
        highlighted_pI.emit ( ikvHlo->m_itemList.at ( ikvHlo->m_nCurItemIndex ) );
        highlighted_pi.emit ( ikvHlo->m_nCurItemIndex );
        ikvHlo->m_bSendHighLighted = false;
    }

    if ( ikvHlo->m_bIsNeedShowItemInfo )
    {
        ikvHlo->m_timerForShowItemInfo.start ( 1000, true );
        ikvHlo->m_bIsNeedShowItemInfo = false;
    }
}

bool GCtrlIconView::fwKeyPressEvent ( GKeyEvent* e )
{
    mpFocus = ikvHlo->m_itemList.at ( ikvHlo->m_nCurItemIndex );
    if ( mpFocus )
    {
        if ( true == mpFocus->fwKeyPress ( e ) )
        {
            return true;
        }
    }

    int nKey=e->key();
    bool bRetVal = true;
    switch ( nKey )
    {
    case Giveda::Key_Left:
        bRetVal = moveFocusLeft();
        break;
    case Giveda::Key_Right:
        bRetVal = moveFocusRight();
        break;
    case Giveda::Key_Up:
        bRetVal = moveFocusUp();
        break;
    case Giveda::Key_Down:
        bRetVal = moveFocusDown();
        break;
    default:
        bRetVal = false;
        break;
    }

    return bRetVal;
}

void GCtrlIconView::insertItem ( GCtrlIconViewItem* pItem, int index )
{
    if ( index<0 )
    {
        ikvHlo->m_itemList.append ( pItem );
    }
    else
    {
        ikvHlo->m_itemList.insert ( index, pItem );
    }
}

void GCtrlIconView::removeItem ( int index )
{
    ikvHlo->m_itemList.remove ( index );
    ikvHlo->m_bIsNeedDoLayout = true;
}

void GCtrlIconView::changeItem ( GCtrlIconViewItem *pItem, int  index )
{
    if ( index == -1 )
    {
        index = ikvHlo->m_nCurItemIndex;
    }

    ikvHlo->m_itemList.remove ( index );
    ikvHlo->m_itemList.insert ( index, pItem );
}

void GCtrlIconView::setRowNums ( unsigned int num )
{
    ikvHlo->m_nRowNums = num;
    ikvHlo->m_nMaxHofItem = height() /ikvHlo->m_nRowNums;
    ikvHlo->m_nDrawTo = ikvHlo->m_nColumnNums*ikvHlo->m_nRowNums-1 + ikvHlo->m_nDrawFrom;
}

void GCtrlIconView::setColumnNums ( unsigned int num )
{
    ikvHlo->m_nColumnNums = num;
    ikvHlo->m_nMaxWofItem = width() /ikvHlo->m_nColumnNums;
    ikvHlo->m_nDrawTo = ikvHlo->m_nColumnNums*ikvHlo->m_nRowNums-1 + ikvHlo->m_nDrawFrom;
}

GCtrlIconViewItem* GCtrlIconView::item ( int index )
{
    return ikvHlo->m_itemList.at ( index );
}

unsigned int GCtrlIconView::count()
{
    return ikvHlo->m_itemList.count();
}

void GCtrlIconView::clear()
{
    ikvHlo->m_nDrawFrom = 0;
    ikvHlo->m_nDrawTo = ikvHlo->m_nColumnNums*ikvHlo->m_nRowNums-1 + ikvHlo->m_nDrawFrom;
    ikvHlo->m_nCurItemIndex = 0;
    ikvHlo->m_bIsNeedDoLayout = true;
    ikvHlo->m_bIsNeedShowItemInfo = true;
    ikvHlo->m_bSendHighLighted = true;

    ikvHlo->m_itemList.clear();
}

void GCtrlIconView::setCurItemIndex ( const int nIndex )
{
    ikvHlo->m_nCurItemIndex = nIndex;
    int nCountsPerScreen=ikvHlo->m_nColumnNums*ikvHlo->m_nRowNums;
    int nToScreen=ikvHlo->m_nCurItemIndex/nCountsPerScreen;
    ikvHlo->m_nDrawFrom = nToScreen*nCountsPerScreen;
    ikvHlo->m_nDrawTo = nCountsPerScreen-1 + ikvHlo->m_nDrawFrom;
    ikvHlo->m_bIsNeedDoLayout = true;
    ikvHlo->m_bIsNeedShowItemInfo = true;
    ikvHlo->m_bSendHighLighted = true;
}

void GCtrlIconView::emitSelected ( GCtrlIconViewItem* pItem )
{
    selected_pi.emit ( ikvHlo->m_nCurItemIndex );
    selected_pI.emit ( pItem );
}

void GCtrlIconView::setFocusInImage ( const GImage& img )
{
    ikvHlo->m_imgFocusIn.setImage ( img );
}

void GCtrlIconView::setFocusOutImage ( const GImage& img )
{
    ikvHlo->m_imgFocusOut.setImage ( img );
}

void GCtrlIconView::slotFocusChangedTo()
{
    if ( hasFocus() )
    {
        focusChangedTo_pI.emit ( ikvHlo->m_itemList.at ( ikvHlo->m_nCurItemIndex ) );
        focusChangedTo_pi.emit ( ikvHlo->m_nCurItemIndex );
        ikvHlo->m_itemList.at ( ikvHlo->m_nCurItemIndex )->getTxt()->startScroll();
    }
}

void GCtrlIconView::slotLoseFocus()
{
    ikvHlo->m_itemList.at ( ikvHlo->m_nCurItemIndex )->getTxt()->stopScroll();
}

void GCtrlIconView::slotGetFocus()
{
    if ( ikvHlo->m_itemList.isEmpty() )
        return;
    ikvHlo->m_itemList.at ( ikvHlo->m_nCurItemIndex )->setFocus();
    ikvHlo->m_bIsNeedShowItemInfo = true;
    highlighted_pI.emit ( ikvHlo->m_itemList.at ( ikvHlo->m_nCurItemIndex ) );
    highlighted_pi.emit ( ikvHlo->m_nCurItemIndex );
}

void GCtrlIconView::slotItemClicked ( const GCtrlIconViewItem* pItem )
{
    int nCurItemIndex = ikvHlo->m_itemList.findRef ( pItem );
    if ( -1 != nCurItemIndex )
    {
        selected_pI.emit ( ikvHlo->m_itemList.at ( nCurItemIndex ) );
        selected_pi.emit ( nCurItemIndex );
    }
}

bool GCtrlIconView::moveFocusLeft()
{
    if ( ikvHlo->m_itemList.count() >1 && ikvHlo->m_nCurItemIndex>0 )
    {
        moveFocus ( ikvHlo->m_nCurItemIndex-1 );
        return true;
    }
    else
    {
        return false;
    }
}

bool GCtrlIconView::moveFocusRight()
{
    if ( ikvHlo->m_itemList.count() >1 && ( unsigned int ) ikvHlo->m_nCurItemIndex<ikvHlo->m_itemList.count()-1 )
    {
        moveFocus ( ikvHlo->m_nCurItemIndex+1 );
        return true;
    }

    return false;
}

bool GCtrlIconView::moveFocusUp()
{
    if ( (int)ikvHlo->m_nCurItemIndex -  ikvHlo->m_nColumnNums >=0 )
    {
        moveFocus ( ikvHlo->m_nCurItemIndex-ikvHlo->m_nColumnNums );
        return true;
    }

    return false;
}

bool GCtrlIconView::moveFocusDown()
{
    if ( ikvHlo->m_nCurItemIndex + ikvHlo->m_nColumnNums <  ikvHlo->m_itemList.count() )
    {
        moveFocus ( ikvHlo->m_nCurItemIndex+ikvHlo->m_nColumnNums );
        return true;
    }
    else if ( ikvHlo->m_nCurItemIndex !=  ikvHlo->m_itemList.count()-1
              && ikvHlo->m_itemList.count()-ikvHlo->m_nCurItemIndex > ikvHlo->m_itemList.count() %ikvHlo->m_nColumnNums )
    {

        moveFocus ( ikvHlo->m_itemList.count()-1 );
        return true;
    }

    return false;
}

void GCtrlIconView::moveFocus ( int toIndex )
{
    int fromIndex = ikvHlo->m_nCurItemIndex;
    int nCountsPerScreen=ikvHlo->m_nColumnNums*ikvHlo->m_nRowNums;
    int nFromScreen=fromIndex/nCountsPerScreen;
    int nToScreen=toIndex/nCountsPerScreen;
    if ( nFromScreen != nToScreen )
    {
        ikvHlo->m_bIsNeedDoLayout = true;
        ikvHlo->m_nDrawFrom = nToScreen*nCountsPerScreen;
        ikvHlo->m_nDrawTo = ikvHlo->m_nDrawFrom+nCountsPerScreen-1;
    }

    ikvHlo->m_itemList.at ( fromIndex )->getTxt()->stopScroll();
    ikvHlo->m_itemList.at ( fromIndex )->getTxt()->setVisible ( false );

    if ( !ikvHlo->m_bIsNeedDoLayout )
    {
        ikvHlo->m_itemList.at ( fromIndex )->update();
        ikvHlo->m_itemList.at ( toIndex )->update();
    }
    else
    {
        update();
    }

    highlighted_pI.emit ( ikvHlo->m_itemList.at ( toIndex ) );
    
    highlighted_pi.emit ( toIndex );
    ikvHlo->m_nCurItemIndex = toIndex;
    ikvHlo->m_bIsNeedShowItemInfo = true;
}

int GCtrlIconView::getMaxIconHeight()
{
    return ikvHlo->m_nMaxHofItem;
}

int GCtrlIconView::getMaxIconWidth()
{
    return ikvHlo->m_nMaxWofItem;
}

// reborn, a nice day ^_^
