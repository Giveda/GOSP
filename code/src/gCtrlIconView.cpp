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

class GCtrlIconViewMhL
{
public:
    GCtrlIconViewMhL( GCtrlForm* frm, GMItem* parent, GCtrlIconView *ikv )
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
    ikvLqH = new GCtrlIconViewMhL(frm, parent, this);
    frm->appendItem ( this );
    ikvLqH->m_nDrawFrom = 0;
    ikvLqH->m_nRowNums = 3;
    setColumnNums ( 3 );
    setRowNums ( 3 );

    connect ( &ikvLqH->m_timerForShowItemInfo,
              ikvLqH->m_timerForShowItemInfo.timeout, this, &GCtrlIconView::slotFocusChangedTo );
    connect ( this, this->loseFocus, this, &GCtrlIconView::slotLoseFocus );
    connect ( this, this->getFocus, this, &GCtrlIconView::slotGetFocus );

    ikvLqH->m_nCurItemIndex = 0;
    ikvLqH->m_bIsNeedDoLayout = true;
    ikvLqH->m_bIsNeedShowItemInfo = true;
    ikvLqH->m_bSendHighLighted = false;

    GCtrlDefaultAppStyle* pAppStyle = getDefaultAppStyle();
    GCtrlItemStyle* pStyle=NULL;
    while ( NULL== ( pStyle=pAppStyle->itemStyle ( className() ) ) )
    {
        pAppStyle->appendIconViewStyle();
    }

    ikvLqH->m_imgFocusIn.setImage ( pStyle->pixmap ( ikvLqH->m_imgFocusIn.name() ) );
    ikvLqH->m_imgFocusOut.setImage ( pStyle->pixmap ( ikvLqH->m_imgFocusOut.name() ) );

    ikvLqH->m_itemList.setAutoDelete ( true );
}

GCtrlIconView::~GCtrlIconView()
{
    ikvLqH->m_itemList.clear();
    delete ikvLqH;
}

void GCtrlIconView::paintEvent ( GPainter& p )
{
    if ( ikvLqH->m_itemList.isEmpty() )
    {
        return;
    }

    int nDrawTo = ikvLqH->m_itemList.count()-1<ikvLqH->m_nDrawTo ? ikvLqH->m_itemList.count()-1 : ikvLqH->m_nDrawTo;
    GCtrlIconViewItem* pItem=NULL;
    if ( ikvLqH->m_bIsNeedDoLayout )
    {
        ikvLqH->m_itemList.at ( ikvLqH->m_nDrawFrom )->setGeometry ( 0, 0, ikvLqH->m_nMaxWofItem, ikvLqH->m_nMaxHofItem );
        int nRightEdge = ikvLqH->m_nMaxWofItem* ( ikvLqH->m_nColumnNums-1 );
        for ( int j=ikvLqH->m_nDrawFrom; j<nDrawTo; j++ )
        {
            pItem = ikvLqH->m_itemList.at ( j+1 );
            if ( ikvLqH->m_itemList.at ( j )->x() == nRightEdge )
            {
                pItem->setGeometry ( 0, ikvLqH->m_itemList.at ( j )->y() +ikvLqH->m_nMaxHofItem, ikvLqH->m_nMaxWofItem, ikvLqH->m_nMaxHofItem );
            }
            else
            {
                pItem->setGeometry ( ikvLqH->m_itemList.at ( j )->x() +ikvLqH->m_nMaxWofItem, ikvLqH->m_itemList.at ( j )->y(), ikvLqH->m_nMaxWofItem, ikvLqH->m_nMaxHofItem );
            }
        }

        ikvLqH->m_bIsNeedDoLayout = false;
    }

    ikvLqH->m_itemList.at ( ikvLqH->m_nCurItemIndex )->getTxt()->setVisible ( true );

    for ( int j=ikvLqH->m_nDrawFrom; j<=nDrawTo; j++ )
    {
        pItem = ikvLqH->m_itemList.at ( j );

        p.save();
        p.translate ( pItem->x(), pItem->y() );
        pItem->draw ( p );
        p.restore();
    }

    GMImage* pIcon=ikvLqH->m_itemList.at ( ikvLqH->m_nCurItemIndex )->getIcon();
    if ( hasFocus() )
    {
        ikvLqH->m_imgFocusIn.setGeometry ( ikvLqH->m_itemList.at ( ikvLqH->m_nCurItemIndex )->x(), ikvLqH->m_itemList.at ( ikvLqH->m_nCurItemIndex )->y(), ikvLqH->m_nMaxWofItem, pIcon->height() );
        ikvLqH->m_imgFocusIn.draw ( p );
    }
    else
    {
        ikvLqH->m_imgFocusOut.setGeometry ( ikvLqH->m_itemList.at ( ikvLqH->m_nCurItemIndex )->x(), ikvLqH->m_itemList.at ( ikvLqH->m_nCurItemIndex )->y(), ikvLqH->m_nMaxWofItem, pIcon->height() );
        ikvLqH->m_imgFocusOut.draw ( p );
    }

    if ( ikvLqH->m_bSendHighLighted )
    {
        highlighted_pI.emit ( ikvLqH->m_itemList.at ( ikvLqH->m_nCurItemIndex ) );
        highlighted_pi.emit ( ikvLqH->m_nCurItemIndex );
        ikvLqH->m_bSendHighLighted = false;
    }

    if ( ikvLqH->m_bIsNeedShowItemInfo )
    {
        ikvLqH->m_timerForShowItemInfo.start ( 1000, true );
        ikvLqH->m_bIsNeedShowItemInfo = false;
    }
}

bool GCtrlIconView::fwKeyPressEvent ( GKeyEvent* e )
{
    mpFocus = ikvLqH->m_itemList.at ( ikvLqH->m_nCurItemIndex );
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
        ikvLqH->m_itemList.append ( pItem );
    }
    else
    {
        ikvLqH->m_itemList.insert ( index, pItem );
    }
}

void GCtrlIconView::removeItem ( int index )
{
    ikvLqH->m_itemList.remove ( index );
    ikvLqH->m_bIsNeedDoLayout = true;
}

void GCtrlIconView::changeItem ( GCtrlIconViewItem *pItem, int  index )
{
    if ( index == -1 )
    {
        index = ikvLqH->m_nCurItemIndex;
    }

    ikvLqH->m_itemList.remove ( index );
    ikvLqH->m_itemList.insert ( index, pItem );
}

void GCtrlIconView::setRowNums ( unsigned int num )
{
    ikvLqH->m_nRowNums = num;
    ikvLqH->m_nMaxHofItem = height() /ikvLqH->m_nRowNums;
    ikvLqH->m_nDrawTo = ikvLqH->m_nColumnNums*ikvLqH->m_nRowNums-1 + ikvLqH->m_nDrawFrom;
}

void GCtrlIconView::setColumnNums ( unsigned int num )
{
    ikvLqH->m_nColumnNums = num;
    ikvLqH->m_nMaxWofItem = width() /ikvLqH->m_nColumnNums;
    ikvLqH->m_nDrawTo = ikvLqH->m_nColumnNums*ikvLqH->m_nRowNums-1 + ikvLqH->m_nDrawFrom;
}

GCtrlIconViewItem* GCtrlIconView::item ( int index )
{
    return ikvLqH->m_itemList.at ( index );
}

unsigned int GCtrlIconView::count()
{
    return ikvLqH->m_itemList.count();
}

void GCtrlIconView::clear()
{
    ikvLqH->m_nDrawFrom = 0;
    ikvLqH->m_nDrawTo = ikvLqH->m_nColumnNums*ikvLqH->m_nRowNums-1 + ikvLqH->m_nDrawFrom;
    ikvLqH->m_nCurItemIndex = 0;
    ikvLqH->m_bIsNeedDoLayout = true;
    ikvLqH->m_bIsNeedShowItemInfo = true;
    ikvLqH->m_bSendHighLighted = true;

    ikvLqH->m_itemList.clear();
}

void GCtrlIconView::setCurItemIndex ( const int nIndex )
{
    ikvLqH->m_nCurItemIndex = nIndex;
    int nCountsPerScreen=ikvLqH->m_nColumnNums*ikvLqH->m_nRowNums;
    int nToScreen=ikvLqH->m_nCurItemIndex/nCountsPerScreen;
    ikvLqH->m_nDrawFrom = nToScreen*nCountsPerScreen;
    ikvLqH->m_nDrawTo = nCountsPerScreen-1 + ikvLqH->m_nDrawFrom;
    ikvLqH->m_bIsNeedDoLayout = true;
    ikvLqH->m_bIsNeedShowItemInfo = true;
    ikvLqH->m_bSendHighLighted = true;
}

void GCtrlIconView::emitSelected ( GCtrlIconViewItem* pItem )
{
    selected_pi.emit ( ikvLqH->m_nCurItemIndex );
    selected_pI.emit ( pItem );
}

void GCtrlIconView::setFocusInImage ( const GImage& img )
{
    ikvLqH->m_imgFocusIn.setImage ( img );
}

void GCtrlIconView::setFocusOutImage ( const GImage& img )
{
    ikvLqH->m_imgFocusOut.setImage ( img );
}

void GCtrlIconView::slotFocusChangedTo()
{
    if ( hasFocus() )
    {
        focusChangedTo_pI.emit ( ikvLqH->m_itemList.at ( ikvLqH->m_nCurItemIndex ) );
        focusChangedTo_pi.emit ( ikvLqH->m_nCurItemIndex );
        ikvLqH->m_itemList.at ( ikvLqH->m_nCurItemIndex )->getTxt()->startScroll();
    }
}

void GCtrlIconView::slotLoseFocus()
{
    ikvLqH->m_itemList.at ( ikvLqH->m_nCurItemIndex )->getTxt()->stopScroll();
}

void GCtrlIconView::slotGetFocus()
{
    if ( ikvLqH->m_itemList.isEmpty() )
        return;
    ikvLqH->m_itemList.at ( ikvLqH->m_nCurItemIndex )->setFocus();
    ikvLqH->m_bIsNeedShowItemInfo = true;
    highlighted_pI.emit ( ikvLqH->m_itemList.at ( ikvLqH->m_nCurItemIndex ) );
    highlighted_pi.emit ( ikvLqH->m_nCurItemIndex );
}

void GCtrlIconView::slotItemClicked ( const GCtrlIconViewItem* pItem )
{
    int nCurItemIndex = ikvLqH->m_itemList.findRef ( pItem );
    if ( -1 != nCurItemIndex )
    {
        selected_pI.emit ( ikvLqH->m_itemList.at ( nCurItemIndex ) );
        selected_pi.emit ( nCurItemIndex );
    }
}

bool GCtrlIconView::moveFocusLeft()
{
    if ( ikvLqH->m_itemList.count() >1 && ikvLqH->m_nCurItemIndex>0 )
    {
        moveFocus ( ikvLqH->m_nCurItemIndex-1 );
        return true;
    }
    else
    {
        return false;
    }
}

bool GCtrlIconView::moveFocusRight()
{
    if ( ikvLqH->m_itemList.count() >1 && ( unsigned int ) ikvLqH->m_nCurItemIndex<ikvLqH->m_itemList.count()-1 )
    {
        moveFocus ( ikvLqH->m_nCurItemIndex+1 );
        return true;
    }

    return false;
}

bool GCtrlIconView::moveFocusUp()
{
    if ( (int)ikvLqH->m_nCurItemIndex -  ikvLqH->m_nColumnNums >=0 )
    {
        moveFocus ( ikvLqH->m_nCurItemIndex-ikvLqH->m_nColumnNums );
        return true;
    }

    return false;
}

bool GCtrlIconView::moveFocusDown()
{
    if ( ikvLqH->m_nCurItemIndex + ikvLqH->m_nColumnNums <  ikvLqH->m_itemList.count() )
    {
        moveFocus ( ikvLqH->m_nCurItemIndex+ikvLqH->m_nColumnNums );
        return true;
    }
    else if ( ikvLqH->m_nCurItemIndex !=  ikvLqH->m_itemList.count()-1
              && ikvLqH->m_itemList.count()-ikvLqH->m_nCurItemIndex > ikvLqH->m_itemList.count() %ikvLqH->m_nColumnNums )
    {

        moveFocus ( ikvLqH->m_itemList.count()-1 );
        return true;
    }

    return false;
}

void GCtrlIconView::moveFocus ( int toIndex )
{
    int fromIndex = ikvLqH->m_nCurItemIndex;
    int nCountsPerScreen=ikvLqH->m_nColumnNums*ikvLqH->m_nRowNums;
    int nFromScreen=fromIndex/nCountsPerScreen;
    int nToScreen=toIndex/nCountsPerScreen;
    if ( nFromScreen != nToScreen )
    {
        ikvLqH->m_bIsNeedDoLayout = true;
        ikvLqH->m_nDrawFrom = nToScreen*nCountsPerScreen;
        ikvLqH->m_nDrawTo = ikvLqH->m_nDrawFrom+nCountsPerScreen-1;
    }

    ikvLqH->m_itemList.at ( fromIndex )->getTxt()->stopScroll();
    ikvLqH->m_itemList.at ( fromIndex )->getTxt()->setVisible ( false );

    if ( !ikvLqH->m_bIsNeedDoLayout )
    {
        ikvLqH->m_itemList.at ( fromIndex )->update();
        ikvLqH->m_itemList.at ( toIndex )->update();
    }
    else
    {
        update();
    }

    highlighted_pI.emit ( ikvLqH->m_itemList.at ( toIndex ) );
    
    highlighted_pi.emit ( toIndex );
    ikvLqH->m_nCurItemIndex = toIndex;
    ikvLqH->m_bIsNeedShowItemInfo = true;
}

int GCtrlIconView::getMaxIconHeight()
{
    return ikvLqH->m_nMaxHofItem;
}

int GCtrlIconView::getMaxIconWidth()
{
    return ikvLqH->m_nMaxWofItem;
}

// have a nice day ^_^
