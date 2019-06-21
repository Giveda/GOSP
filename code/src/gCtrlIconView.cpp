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

class GCtrlIconViewSelf
{
public:
    GCtrlIconViewSelf( GCtrlForm* frm, GMItem* parent, GCtrlIconView *ikv ): m_imgFocusIn ( frm, parent, "iconViewFocusIn" ), m_imgFocusOut ( frm, parent, "iconViewFocusOut" ), m_timerForShowItemInfo ( ikv )
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
    ikvSpp = new GCtrlIconViewSelf(frm, parent, this);
    frm->appendItem ( this );
    ikvSpp->m_nDrawFrom = 0;
    ikvSpp->m_nRowNums = 3;
    setColumnNums ( 3 );
    setRowNums ( 3 );

    connect ( &ikvSpp->m_timerForShowItemInfo,
              ikvSpp->m_timerForShowItemInfo.timeout, this, &GCtrlIconView::slotFocusChangedTo );
    connect ( this, this->loseFocus, this, &GCtrlIconView::slotLoseFocus );
    connect ( this, this->getFocus, this, &GCtrlIconView::slotGetFocus );

    ikvSpp->m_nCurItemIndex = 0;
    ikvSpp->m_bIsNeedDoLayout = true;
    ikvSpp->m_bIsNeedShowItemInfo = true;
    ikvSpp->m_bSendHighLighted = false;

    GCtrlDefaultAppStyle* pAppStyle = getDefaultAppStyle();
    GCtrlItemStyle* pStyle=NULL;
    while ( NULL== ( pStyle=pAppStyle->itemStyle ( className() ) ) )
    {
        pAppStyle->appendIconViewStyle();
    }

    ikvSpp->m_imgFocusIn.setImage ( pStyle->pixmap ( ikvSpp->m_imgFocusIn.name() ) );
    ikvSpp->m_imgFocusOut.setImage ( pStyle->pixmap ( ikvSpp->m_imgFocusOut.name() ) );

    ikvSpp->m_itemList.setAutoDelete ( true );
}

GCtrlIconView::~GCtrlIconView()
{
    ikvSpp->m_itemList.clear();
    delete ikvSpp;
}

void GCtrlIconView::paintEvent ( GPainter& p )
{
    if ( ikvSpp->m_itemList.isEmpty() )
    {
        return;
    }

    int nDrawTo = ikvSpp->m_itemList.count()-1<ikvSpp->m_nDrawTo ? ikvSpp->m_itemList.count()-1 : ikvSpp->m_nDrawTo;
    GCtrlIconViewItem* pItem=NULL;
    if ( ikvSpp->m_bIsNeedDoLayout )
    {
        ikvSpp->m_itemList.at ( ikvSpp->m_nDrawFrom )->setGeometry ( 0, 0, ikvSpp->m_nMaxWofItem, ikvSpp->m_nMaxHofItem );
        int nRightEdge = ikvSpp->m_nMaxWofItem* ( ikvSpp->m_nColumnNums-1 );
        for ( int j=ikvSpp->m_nDrawFrom; j<nDrawTo; j++ )
        {
            pItem = ikvSpp->m_itemList.at ( j+1 );
            if ( ikvSpp->m_itemList.at ( j )->x() == nRightEdge )
            {
                pItem->setGeometry ( 0, ikvSpp->m_itemList.at ( j )->y() +ikvSpp->m_nMaxHofItem, ikvSpp->m_nMaxWofItem, ikvSpp->m_nMaxHofItem );
            }
            else
            {
                pItem->setGeometry ( ikvSpp->m_itemList.at ( j )->x() +ikvSpp->m_nMaxWofItem, ikvSpp->m_itemList.at ( j )->y(), ikvSpp->m_nMaxWofItem, ikvSpp->m_nMaxHofItem );
            }
        }

        ikvSpp->m_bIsNeedDoLayout = false;
    }

    ikvSpp->m_itemList.at ( ikvSpp->m_nCurItemIndex )->getTxt()->setVisible ( true );

    for ( int j=ikvSpp->m_nDrawFrom; j<=nDrawTo; j++ )
    {
        pItem = ikvSpp->m_itemList.at ( j );

        p.save();
        p.translate ( pItem->x(), pItem->y() );
        pItem->draw ( p );
        p.restore();
    }

    GMImage* pIcon=ikvSpp->m_itemList.at ( ikvSpp->m_nCurItemIndex )->getIcon();
    if ( hasFocus() )
    {
        ikvSpp->m_imgFocusIn.setGeometry ( ikvSpp->m_itemList.at ( ikvSpp->m_nCurItemIndex )->x(), ikvSpp->m_itemList.at ( ikvSpp->m_nCurItemIndex )->y(), ikvSpp->m_nMaxWofItem, pIcon->height() );
        ikvSpp->m_imgFocusIn.draw ( p );
    }
    else
    {
        ikvSpp->m_imgFocusOut.setGeometry ( ikvSpp->m_itemList.at ( ikvSpp->m_nCurItemIndex )->x(), ikvSpp->m_itemList.at ( ikvSpp->m_nCurItemIndex )->y(), ikvSpp->m_nMaxWofItem, pIcon->height() );
        ikvSpp->m_imgFocusOut.draw ( p );
    }

    if ( ikvSpp->m_bSendHighLighted )
    {
        highlighted_pI.emit ( ikvSpp->m_itemList.at ( ikvSpp->m_nCurItemIndex ) );
        highlighted_pi.emit ( ikvSpp->m_nCurItemIndex );
        ikvSpp->m_bSendHighLighted = false;
    }

    if ( ikvSpp->m_bIsNeedShowItemInfo )
    {
        ikvSpp->m_timerForShowItemInfo.start ( 1000, true );
        ikvSpp->m_bIsNeedShowItemInfo = false;
    }
}

bool GCtrlIconView::fwKeyPressEvent ( GKeyEvent* e )
{
    mpFocus = ikvSpp->m_itemList.at ( ikvSpp->m_nCurItemIndex );
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
        ikvSpp->m_itemList.append ( pItem );
    }
    else
    {
        ikvSpp->m_itemList.insert ( index, pItem );
    }
}

void GCtrlIconView::removeItem ( int index )
{
    ikvSpp->m_itemList.remove ( index );
    ikvSpp->m_bIsNeedDoLayout = true;
}

void GCtrlIconView::changeItem ( GCtrlIconViewItem *pItem, int  index )
{
    if ( index == -1 )
    {
        index = ikvSpp->m_nCurItemIndex;
    }

    ikvSpp->m_itemList.remove ( index );
    ikvSpp->m_itemList.insert ( index, pItem );
}

void GCtrlIconView::setRowNums ( unsigned int num )
{
    ikvSpp->m_nRowNums = num;
    ikvSpp->m_nMaxHofItem = height() /ikvSpp->m_nRowNums;
    ikvSpp->m_nDrawTo = ikvSpp->m_nColumnNums*ikvSpp->m_nRowNums-1 + ikvSpp->m_nDrawFrom;
}

void GCtrlIconView::setColumnNums ( unsigned int num )
{
    ikvSpp->m_nColumnNums = num;
    ikvSpp->m_nMaxWofItem = width() /ikvSpp->m_nColumnNums;
    ikvSpp->m_nDrawTo = ikvSpp->m_nColumnNums*ikvSpp->m_nRowNums-1 + ikvSpp->m_nDrawFrom;
}

GCtrlIconViewItem* GCtrlIconView::item ( int index )
{
    return ikvSpp->m_itemList.at ( index );
}

unsigned int GCtrlIconView::count()
{
    return ikvSpp->m_itemList.count();
}

void GCtrlIconView::clear()
{
    ikvSpp->m_nDrawFrom = 0;
    ikvSpp->m_nDrawTo = ikvSpp->m_nColumnNums*ikvSpp->m_nRowNums-1 + ikvSpp->m_nDrawFrom;
    ikvSpp->m_nCurItemIndex = 0;
    ikvSpp->m_bIsNeedDoLayout = true;
    ikvSpp->m_bIsNeedShowItemInfo = true;
    ikvSpp->m_bSendHighLighted = true;

    ikvSpp->m_itemList.clear();
}

void GCtrlIconView::setCurItemIndex ( const int nIndex )
{
    ikvSpp->m_nCurItemIndex = nIndex;
    int nCountsPerScreen=ikvSpp->m_nColumnNums*ikvSpp->m_nRowNums;
    int nToScreen=ikvSpp->m_nCurItemIndex/nCountsPerScreen;
    ikvSpp->m_nDrawFrom = nToScreen*nCountsPerScreen;
    ikvSpp->m_nDrawTo = nCountsPerScreen-1 + ikvSpp->m_nDrawFrom;
    ikvSpp->m_bIsNeedDoLayout = true;
    ikvSpp->m_bIsNeedShowItemInfo = true;
    ikvSpp->m_bSendHighLighted = true;
}

void GCtrlIconView::emitSelected ( GCtrlIconViewItem* pItem )
{
    selected_pi.emit ( ikvSpp->m_nCurItemIndex );
    selected_pI.emit ( pItem );
}

void GCtrlIconView::setFocusInImage ( const GImage& img )
{
    ikvSpp->m_imgFocusIn.setImage ( img );
}

void GCtrlIconView::setFocusOutImage ( const GImage& img )
{
    ikvSpp->m_imgFocusOut.setImage ( img );
}

void GCtrlIconView::slotFocusChangedTo()
{
    if ( hasFocus() )
    {
        focusChangedTo_pI.emit ( ikvSpp->m_itemList.at ( ikvSpp->m_nCurItemIndex ) );
        focusChangedTo_pi.emit ( ikvSpp->m_nCurItemIndex );
        ikvSpp->m_itemList.at ( ikvSpp->m_nCurItemIndex )->getTxt()->startScroll();
    }
}

void GCtrlIconView::slotLoseFocus()
{
    ikvSpp->m_itemList.at ( ikvSpp->m_nCurItemIndex )->getTxt()->stopScroll();
}

void GCtrlIconView::slotGetFocus()
{
    if ( ikvSpp->m_itemList.isEmpty() )
        return;
    ikvSpp->m_itemList.at ( ikvSpp->m_nCurItemIndex )->setFocus();
    ikvSpp->m_bIsNeedShowItemInfo = true;
    highlighted_pI.emit ( ikvSpp->m_itemList.at ( ikvSpp->m_nCurItemIndex ) );
    highlighted_pi.emit ( ikvSpp->m_nCurItemIndex );
}

void GCtrlIconView::slotItemClicked ( const GCtrlIconViewItem* pItem )
{
    int nCurItemIndex = ikvSpp->m_itemList.findRef ( pItem );
    if ( -1 != nCurItemIndex )
    {
        selected_pI.emit ( ikvSpp->m_itemList.at ( nCurItemIndex ) );
        selected_pi.emit ( nCurItemIndex );
    }
}

bool GCtrlIconView::moveFocusLeft()
{
    if ( ikvSpp->m_itemList.count() >1 && ikvSpp->m_nCurItemIndex>0 )
    {
        moveFocus ( ikvSpp->m_nCurItemIndex-1 );
        return true;
    }
    else
    {
        return false;
    }
}

bool GCtrlIconView::moveFocusRight()
{
    if ( ikvSpp->m_itemList.count() >1 && ( unsigned int ) ikvSpp->m_nCurItemIndex<ikvSpp->m_itemList.count()-1 )
    {
        moveFocus ( ikvSpp->m_nCurItemIndex+1 );
        return true;
    }

    return false;
}

bool GCtrlIconView::moveFocusUp()
{
    if ( (int)ikvSpp->m_nCurItemIndex -  ikvSpp->m_nColumnNums >=0 )
    {
        moveFocus ( ikvSpp->m_nCurItemIndex-ikvSpp->m_nColumnNums );
        return true;
    }

    return false;
}

bool GCtrlIconView::moveFocusDown()
{
    if ( ikvSpp->m_nCurItemIndex + ikvSpp->m_nColumnNums <  ikvSpp->m_itemList.count() )
    {
        moveFocus ( ikvSpp->m_nCurItemIndex+ikvSpp->m_nColumnNums );
        return true;
    }
    else if ( ikvSpp->m_nCurItemIndex !=  ikvSpp->m_itemList.count()-1
              && ikvSpp->m_itemList.count()-ikvSpp->m_nCurItemIndex > ikvSpp->m_itemList.count() %ikvSpp->m_nColumnNums )
    {

        moveFocus ( ikvSpp->m_itemList.count()-1 );
        return true;
    }

    return false;
}

void GCtrlIconView::moveFocus ( int toIndex )
{
    int fromIndex = ikvSpp->m_nCurItemIndex;
    int nCountsPerScreen=ikvSpp->m_nColumnNums*ikvSpp->m_nRowNums;
    int nFromScreen=fromIndex/nCountsPerScreen;
    int nToScreen=toIndex/nCountsPerScreen;
    if ( nFromScreen != nToScreen )
    {
        ikvSpp->m_bIsNeedDoLayout = true;
        ikvSpp->m_nDrawFrom = nToScreen*nCountsPerScreen;
        ikvSpp->m_nDrawTo = ikvSpp->m_nDrawFrom+nCountsPerScreen-1;
    }

    ikvSpp->m_itemList.at ( fromIndex )->getTxt()->stopScroll();
    ikvSpp->m_itemList.at ( fromIndex )->getTxt()->setVisible ( false );

    if ( !ikvSpp->m_bIsNeedDoLayout )
    {
        ikvSpp->m_itemList.at ( fromIndex )->update();
        ikvSpp->m_itemList.at ( toIndex )->update();
    }
    else
    {
        update();
    }

    highlighted_pI.emit ( ikvSpp->m_itemList.at ( toIndex ) );
    highlighted_pi.emit ( toIndex );
    ikvSpp->m_nCurItemIndex = toIndex;
    ikvSpp->m_bIsNeedShowItemInfo = true;
}
int GCtrlIconView::getMaxIconHeight()
{
    return ikvSpp->m_nMaxHofItem;
}
int GCtrlIconView::getMaxIconWidth()
{
    return ikvSpp->m_nMaxWofItem;
}

// have a nice day ^_^
// have a nice day ^_^
