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

class GCtrlIconViewHmq
{
public:
    GCtrlIconViewHmq( GCtrlForm* frm, GMItem* parent, GCtrlIconView *ikv )
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
    ikvMqs = new GCtrlIconViewHmq(frm, parent, this);
    frm->appendItem ( this );
    ikvMqs->m_nDrawFrom = 0;
    ikvMqs->m_nRowNums = 3;
    setColumnNums ( 3 );
    setRowNums ( 3 );

    connect ( &ikvMqs->m_timerForShowItemInfo,
              ikvMqs->m_timerForShowItemInfo.timeout, this, &GCtrlIconView::slotFocusChangedTo );
    connect ( this, this->loseFocus, this, &GCtrlIconView::slotLoseFocus );
    connect ( this, this->getFocus, this, &GCtrlIconView::slotGetFocus );

    ikvMqs->m_nCurItemIndex = 0;
    ikvMqs->m_bIsNeedDoLayout = true;
    ikvMqs->m_bIsNeedShowItemInfo = true;
    ikvMqs->m_bSendHighLighted = false;

    GCtrlDefaultAppStyle* pAppStyle = getDefaultAppStyle();
    GCtrlItemStyle* pStyle=NULL;
    while ( NULL== ( pStyle=pAppStyle->itemStyle ( className() ) ) )
    {
        pAppStyle->appendIconViewStyle();
    }

    ikvMqs->m_imgFocusIn.setImage ( pStyle->pixmap ( ikvMqs->m_imgFocusIn.name() ) );
    ikvMqs->m_imgFocusOut.setImage ( pStyle->pixmap ( ikvMqs->m_imgFocusOut.name() ) );

    ikvMqs->m_itemList.setAutoDelete ( true );
}

GCtrlIconView::~GCtrlIconView()
{
    ikvMqs->m_itemList.clear();
    delete ikvMqs;
}

void GCtrlIconView::paintEvent ( GPainter& p )
{
    if ( ikvMqs->m_itemList.isEmpty() )
    {
        return;
    }

    int nDrawTo = ikvMqs->m_itemList.count()-1<ikvMqs->m_nDrawTo ? ikvMqs->m_itemList.count()-1 : ikvMqs->m_nDrawTo;
    GCtrlIconViewItem* pItem=NULL;
    if ( ikvMqs->m_bIsNeedDoLayout )
    {
        ikvMqs->m_itemList.at ( ikvMqs->m_nDrawFrom )->setGeometry ( 0, 0, ikvMqs->m_nMaxWofItem, ikvMqs->m_nMaxHofItem );
        int nRightEdge = ikvMqs->m_nMaxWofItem* ( ikvMqs->m_nColumnNums-1 );
        for ( int j=ikvMqs->m_nDrawFrom; j<nDrawTo; j++ )
        {
            pItem = ikvMqs->m_itemList.at ( j+1 );
            if ( ikvMqs->m_itemList.at ( j )->x() == nRightEdge )
            {
                pItem->setGeometry ( 0, ikvMqs->m_itemList.at ( j )->y() +ikvMqs->m_nMaxHofItem, ikvMqs->m_nMaxWofItem, ikvMqs->m_nMaxHofItem );
            }
            else
            {
                pItem->setGeometry ( ikvMqs->m_itemList.at ( j )->x() +ikvMqs->m_nMaxWofItem, ikvMqs->m_itemList.at ( j )->y(), ikvMqs->m_nMaxWofItem, ikvMqs->m_nMaxHofItem );
            }
        }

        ikvMqs->m_bIsNeedDoLayout = false;
    }

    ikvMqs->m_itemList.at ( ikvMqs->m_nCurItemIndex )->getTxt()->setVisible ( true );

    for ( int j=ikvMqs->m_nDrawFrom; j<=nDrawTo; j++ )
    {
        pItem = ikvMqs->m_itemList.at ( j );

        p.save();
        p.translate ( pItem->x(), pItem->y() );
        pItem->draw ( p );
        p.restore();
    }

    GMImage* pIcon=ikvMqs->m_itemList.at ( ikvMqs->m_nCurItemIndex )->getIcon();
    if ( hasFocus() )
    {
        ikvMqs->m_imgFocusIn.setGeometry ( ikvMqs->m_itemList.at ( ikvMqs->m_nCurItemIndex )->x(), ikvMqs->m_itemList.at ( ikvMqs->m_nCurItemIndex )->y(), ikvMqs->m_nMaxWofItem, pIcon->height() );
        ikvMqs->m_imgFocusIn.draw ( p );
    }
    else
    {
        ikvMqs->m_imgFocusOut.setGeometry ( ikvMqs->m_itemList.at ( ikvMqs->m_nCurItemIndex )->x(), ikvMqs->m_itemList.at ( ikvMqs->m_nCurItemIndex )->y(), ikvMqs->m_nMaxWofItem, pIcon->height() );
        ikvMqs->m_imgFocusOut.draw ( p );
    }

    if ( ikvMqs->m_bSendHighLighted )
    {
        highlighted_pI.emit ( ikvMqs->m_itemList.at ( ikvMqs->m_nCurItemIndex ) );
        highlighted_pi.emit ( ikvMqs->m_nCurItemIndex );
        ikvMqs->m_bSendHighLighted = false;
    }

    if ( ikvMqs->m_bIsNeedShowItemInfo )
    {
        ikvMqs->m_timerForShowItemInfo.start ( 1000, true );
        ikvMqs->m_bIsNeedShowItemInfo = false;
    }
}

bool GCtrlIconView::fwKeyPressEvent ( GKeyEvent* e )
{
    mpFocus = ikvMqs->m_itemList.at ( ikvMqs->m_nCurItemIndex );
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
        ikvMqs->m_itemList.append ( pItem );
    }
    else
    {
        ikvMqs->m_itemList.insert ( index, pItem );
    }
}

void GCtrlIconView::removeItem ( int index )
{
    ikvMqs->m_itemList.remove ( index );
    ikvMqs->m_bIsNeedDoLayout = true;
}

void GCtrlIconView::changeItem ( GCtrlIconViewItem *pItem, int  index )
{
    if ( index == -1 )
    {
        index = ikvMqs->m_nCurItemIndex;
    }

    ikvMqs->m_itemList.remove ( index );
    ikvMqs->m_itemList.insert ( index, pItem );
}

void GCtrlIconView::setRowNums ( unsigned int num )
{
    ikvMqs->m_nRowNums = num;
    ikvMqs->m_nMaxHofItem = height() /ikvMqs->m_nRowNums;
    ikvMqs->m_nDrawTo = ikvMqs->m_nColumnNums*ikvMqs->m_nRowNums-1 + ikvMqs->m_nDrawFrom;
}

void GCtrlIconView::setColumnNums ( unsigned int num )
{
    ikvMqs->m_nColumnNums = num;
    ikvMqs->m_nMaxWofItem = width() /ikvMqs->m_nColumnNums;
    ikvMqs->m_nDrawTo = ikvMqs->m_nColumnNums*ikvMqs->m_nRowNums-1 + ikvMqs->m_nDrawFrom;
}

GCtrlIconViewItem* GCtrlIconView::item ( int index )
{
    return ikvMqs->m_itemList.at ( index );
}

unsigned int GCtrlIconView::count()
{
    return ikvMqs->m_itemList.count();
}

void GCtrlIconView::clear()
{
    ikvMqs->m_nDrawFrom = 0;
    ikvMqs->m_nDrawTo = ikvMqs->m_nColumnNums*ikvMqs->m_nRowNums-1 + ikvMqs->m_nDrawFrom;
    ikvMqs->m_nCurItemIndex = 0;
    ikvMqs->m_bIsNeedDoLayout = true;
    ikvMqs->m_bIsNeedShowItemInfo = true;
    ikvMqs->m_bSendHighLighted = true;

    ikvMqs->m_itemList.clear();
}

void GCtrlIconView::setCurItemIndex ( const int nIndex )
{
    ikvMqs->m_nCurItemIndex = nIndex;
    int nCountsPerScreen=ikvMqs->m_nColumnNums*ikvMqs->m_nRowNums;
    int nToScreen=ikvMqs->m_nCurItemIndex/nCountsPerScreen;
    ikvMqs->m_nDrawFrom = nToScreen*nCountsPerScreen;
    ikvMqs->m_nDrawTo = nCountsPerScreen-1 + ikvMqs->m_nDrawFrom;
    ikvMqs->m_bIsNeedDoLayout = true;
    ikvMqs->m_bIsNeedShowItemInfo = true;
    ikvMqs->m_bSendHighLighted = true;
}

void GCtrlIconView::emitSelected ( GCtrlIconViewItem* pItem )
{
    selected_pi.emit ( ikvMqs->m_nCurItemIndex );
    selected_pI.emit ( pItem );
}

void GCtrlIconView::setFocusInImage ( const GImage& img )
{
    ikvMqs->m_imgFocusIn.setImage ( img );
}

void GCtrlIconView::setFocusOutImage ( const GImage& img )
{
    ikvMqs->m_imgFocusOut.setImage ( img );
}

void GCtrlIconView::slotFocusChangedTo()
{
    if ( hasFocus() )
    {
        focusChangedTo_pI.emit ( ikvMqs->m_itemList.at ( ikvMqs->m_nCurItemIndex ) );
        focusChangedTo_pi.emit ( ikvMqs->m_nCurItemIndex );
        ikvMqs->m_itemList.at ( ikvMqs->m_nCurItemIndex )->getTxt()->startScroll();
    }
}

void GCtrlIconView::slotLoseFocus()
{
    ikvMqs->m_itemList.at ( ikvMqs->m_nCurItemIndex )->getTxt()->stopScroll();
}

void GCtrlIconView::slotGetFocus()
{
    if ( ikvMqs->m_itemList.isEmpty() )
        return;
    ikvMqs->m_itemList.at ( ikvMqs->m_nCurItemIndex )->setFocus();
    ikvMqs->m_bIsNeedShowItemInfo = true;
    highlighted_pI.emit ( ikvMqs->m_itemList.at ( ikvMqs->m_nCurItemIndex ) );
    highlighted_pi.emit ( ikvMqs->m_nCurItemIndex );
}

void GCtrlIconView::slotItemClicked ( const GCtrlIconViewItem* pItem )
{
    int nCurItemIndex = ikvMqs->m_itemList.findRef ( pItem );
    if ( -1 != nCurItemIndex )
    {
        selected_pI.emit ( ikvMqs->m_itemList.at ( nCurItemIndex ) );
        selected_pi.emit ( nCurItemIndex );
    }
}

bool GCtrlIconView::moveFocusLeft()
{
    if ( ikvMqs->m_itemList.count() >1 && ikvMqs->m_nCurItemIndex>0 )
    {
        moveFocus ( ikvMqs->m_nCurItemIndex-1 );
        return true;
    }
    else
    {
        return false;
    }
}

bool GCtrlIconView::moveFocusRight()
{
    if ( ikvMqs->m_itemList.count() >1 && ( unsigned int ) ikvMqs->m_nCurItemIndex<ikvMqs->m_itemList.count()-1 )
    {
        moveFocus ( ikvMqs->m_nCurItemIndex+1 );
        return true;
    }

    return false;
}

bool GCtrlIconView::moveFocusUp()
{
    if ( (int)ikvMqs->m_nCurItemIndex -  ikvMqs->m_nColumnNums >=0 )
    {
        moveFocus ( ikvMqs->m_nCurItemIndex-ikvMqs->m_nColumnNums );
        return true;
    }

    return false;
}

bool GCtrlIconView::moveFocusDown()
{
    if ( ikvMqs->m_nCurItemIndex + ikvMqs->m_nColumnNums <  ikvMqs->m_itemList.count() )
    {
        moveFocus ( ikvMqs->m_nCurItemIndex+ikvMqs->m_nColumnNums );
        return true;
    }
    else if ( ikvMqs->m_nCurItemIndex !=  ikvMqs->m_itemList.count()-1
              && ikvMqs->m_itemList.count()-ikvMqs->m_nCurItemIndex > ikvMqs->m_itemList.count() %ikvMqs->m_nColumnNums )
    {

        moveFocus ( ikvMqs->m_itemList.count()-1 );
        return true;
    }

    return false;
}

void GCtrlIconView::moveFocus ( int toIndex )
{
    int fromIndex = ikvMqs->m_nCurItemIndex;
    int nCountsPerScreen=ikvMqs->m_nColumnNums*ikvMqs->m_nRowNums;
    int nFromScreen=fromIndex/nCountsPerScreen;
    int nToScreen=toIndex/nCountsPerScreen;
    if ( nFromScreen != nToScreen )
    {
        ikvMqs->m_bIsNeedDoLayout = true;
        ikvMqs->m_nDrawFrom = nToScreen*nCountsPerScreen;
        ikvMqs->m_nDrawTo = ikvMqs->m_nDrawFrom+nCountsPerScreen-1;
    }

    ikvMqs->m_itemList.at ( fromIndex )->getTxt()->stopScroll();
    ikvMqs->m_itemList.at ( fromIndex )->getTxt()->setVisible ( false );

    if ( !ikvMqs->m_bIsNeedDoLayout )
    {
        ikvMqs->m_itemList.at ( fromIndex )->update();
        ikvMqs->m_itemList.at ( toIndex )->update();
    }
    else
    {
        update();
    }

    highlighted_pI.emit ( ikvMqs->m_itemList.at ( toIndex ) );
    
    highlighted_pi.emit ( toIndex );
    ikvMqs->m_nCurItemIndex = toIndex;
    ikvMqs->m_bIsNeedShowItemInfo = true;
}

int GCtrlIconView::getMaxIconHeight()
{
    return ikvMqs->m_nMaxHofItem;
}

int GCtrlIconView::getMaxIconWidth()
{
    return ikvMqs->m_nMaxWofItem;
}

// have a nice day ^_^
// have a nice day ^_^
