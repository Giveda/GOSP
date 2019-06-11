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

class GCtrlIconViewPrivate
{
public:
    GCtrlIconViewPrivate( GCtrlForm* frm, GMItem* parent, GCtrlIconView *ikv ): m_imgFocusIn ( frm, parent, "iconViewFocusIn" ), m_imgFocusOut ( frm, parent, "iconViewFocusOut" ), m_timerForShowItemInfo ( ikv )
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
    ikvPriv = new GCtrlIconViewPrivate(frm, parent, this);
    frm->appendItem ( this );
    ikvPriv->m_nDrawFrom = 0;
    ikvPriv->m_nRowNums = 3;
    setColumnNums ( 3 );
    setRowNums ( 3 );

    connect ( &ikvPriv->m_timerForShowItemInfo,
              ikvPriv->m_timerForShowItemInfo.timeout, this, &GCtrlIconView::slotFocusChangedTo );
    connect ( this, this->loseFocus, this, &GCtrlIconView::slotLoseFocus );
    connect ( this, this->getFocus, this, &GCtrlIconView::slotGetFocus );

    ikvPriv->m_nCurItemIndex = 0;
    ikvPriv->m_bIsNeedDoLayout = true;
    ikvPriv->m_bIsNeedShowItemInfo = true;
    ikvPriv->m_bSendHighLighted = false;

    GCtrlDefaultAppStyle* pAppStyle = getDefaultAppStyle();
    GCtrlItemStyle* pStyle=NULL;
    while ( NULL== ( pStyle=pAppStyle->itemStyle ( className() ) ) )
    {
        pAppStyle->appendIconViewStyle();
    }

    ikvPriv->m_imgFocusIn.setImage ( pStyle->pixmap ( ikvPriv->m_imgFocusIn.name() ) );
    ikvPriv->m_imgFocusOut.setImage ( pStyle->pixmap ( ikvPriv->m_imgFocusOut.name() ) );

    ikvPriv->m_itemList.setAutoDelete ( true );
}

GCtrlIconView::~GCtrlIconView()
{
    ikvPriv->m_itemList.clear();
    delete ikvPriv;
}

void GCtrlIconView::paintEvent ( GPainter& p )
{
    if ( ikvPriv->m_itemList.isEmpty() )
    {
        return;
    }

    int nDrawTo = ikvPriv->m_itemList.count()-1<ikvPriv->m_nDrawTo ? ikvPriv->m_itemList.count()-1 : ikvPriv->m_nDrawTo;
    GCtrlIconViewItem* pItem=NULL;
    if ( ikvPriv->m_bIsNeedDoLayout )
    {
        ikvPriv->m_itemList.at ( ikvPriv->m_nDrawFrom )->setGeometry ( 0, 0, ikvPriv->m_nMaxWofItem, ikvPriv->m_nMaxHofItem );
        int nRightEdge = ikvPriv->m_nMaxWofItem* ( ikvPriv->m_nColumnNums-1 );
        for ( int j=ikvPriv->m_nDrawFrom; j<nDrawTo; j++ )
        {
            pItem = ikvPriv->m_itemList.at ( j+1 );
            if ( ikvPriv->m_itemList.at ( j )->x() == nRightEdge )
            {
                pItem->setGeometry ( 0, ikvPriv->m_itemList.at ( j )->y() +ikvPriv->m_nMaxHofItem, ikvPriv->m_nMaxWofItem, ikvPriv->m_nMaxHofItem );
            }
            else
            {
                pItem->setGeometry ( ikvPriv->m_itemList.at ( j )->x() +ikvPriv->m_nMaxWofItem, ikvPriv->m_itemList.at ( j )->y(), ikvPriv->m_nMaxWofItem, ikvPriv->m_nMaxHofItem );
            }
        }

        ikvPriv->m_bIsNeedDoLayout = false;
    }

    ikvPriv->m_itemList.at ( ikvPriv->m_nCurItemIndex )->getTxt()->setVisible ( true );

    for ( int j=ikvPriv->m_nDrawFrom; j<=nDrawTo; j++ )
    {
        pItem = ikvPriv->m_itemList.at ( j );

        p.save();
        p.translate ( pItem->x(), pItem->y() );
        pItem->draw ( p );
        p.restore();
    }

    GMImage* pIcon=ikvPriv->m_itemList.at ( ikvPriv->m_nCurItemIndex )->getIcon();
    if ( hasFocus() )
    {
        ikvPriv->m_imgFocusIn.setGeometry ( ikvPriv->m_itemList.at ( ikvPriv->m_nCurItemIndex )->x(), ikvPriv->m_itemList.at ( ikvPriv->m_nCurItemIndex )->y(), ikvPriv->m_nMaxWofItem, pIcon->height() );
        ikvPriv->m_imgFocusIn.draw ( p );
    }
    else
    {
        ikvPriv->m_imgFocusOut.setGeometry ( ikvPriv->m_itemList.at ( ikvPriv->m_nCurItemIndex )->x(), ikvPriv->m_itemList.at ( ikvPriv->m_nCurItemIndex )->y(), ikvPriv->m_nMaxWofItem, pIcon->height() );
        ikvPriv->m_imgFocusOut.draw ( p );
    }

    if ( ikvPriv->m_bSendHighLighted )
    {
        highlighted_pI.emit ( ikvPriv->m_itemList.at ( ikvPriv->m_nCurItemIndex ) );
        highlighted_pi.emit ( ikvPriv->m_nCurItemIndex );
        ikvPriv->m_bSendHighLighted = false;
    }

    if ( ikvPriv->m_bIsNeedShowItemInfo )
    {
        ikvPriv->m_timerForShowItemInfo.start ( 1000, true );
        ikvPriv->m_bIsNeedShowItemInfo = false;
    }
}

bool GCtrlIconView::fwKeyPressEvent ( GKeyEvent* e )
{
    mpFocus = ikvPriv->m_itemList.at ( ikvPriv->m_nCurItemIndex );
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
        ikvPriv->m_itemList.append ( pItem );
    }
    else
    {
        ikvPriv->m_itemList.insert ( index, pItem );
    }
}

void GCtrlIconView::removeItem ( int index )
{
    ikvPriv->m_itemList.remove ( index );
    ikvPriv->m_bIsNeedDoLayout = true;
}

void GCtrlIconView::changeItem ( GCtrlIconViewItem *pItem, int  index )
{
    if ( index == -1 )
    {
        index = ikvPriv->m_nCurItemIndex;
    }

    ikvPriv->m_itemList.remove ( index );
    ikvPriv->m_itemList.insert ( index, pItem );
}

void GCtrlIconView::setRowNums ( unsigned int num )
{
    ikvPriv->m_nRowNums = num;
    ikvPriv->m_nMaxHofItem = height() /ikvPriv->m_nRowNums;
    ikvPriv->m_nDrawTo = ikvPriv->m_nColumnNums*ikvPriv->m_nRowNums-1 + ikvPriv->m_nDrawFrom;
}

void GCtrlIconView::setColumnNums ( unsigned int num )
{
    ikvPriv->m_nColumnNums = num;
    ikvPriv->m_nMaxWofItem = width() /ikvPriv->m_nColumnNums;
    ikvPriv->m_nDrawTo = ikvPriv->m_nColumnNums*ikvPriv->m_nRowNums-1 + ikvPriv->m_nDrawFrom;
}

GCtrlIconViewItem* GCtrlIconView::item ( int index )
{
    return ikvPriv->m_itemList.at ( index );
}

unsigned int GCtrlIconView::count()
{
    return ikvPriv->m_itemList.count();
}

void GCtrlIconView::clear()
{
    ikvPriv->m_nDrawFrom = 0;
    ikvPriv->m_nDrawTo = ikvPriv->m_nColumnNums*ikvPriv->m_nRowNums-1 + ikvPriv->m_nDrawFrom;
    ikvPriv->m_nCurItemIndex = 0;
    ikvPriv->m_bIsNeedDoLayout = true;
    ikvPriv->m_bIsNeedShowItemInfo = true;
    ikvPriv->m_bSendHighLighted = true;

    ikvPriv->m_itemList.clear();
}

void GCtrlIconView::setCurItemIndex ( const int nIndex )
{
    ikvPriv->m_nCurItemIndex = nIndex;
    int nCountsPerScreen=ikvPriv->m_nColumnNums*ikvPriv->m_nRowNums;
    int nToScreen=ikvPriv->m_nCurItemIndex/nCountsPerScreen;
    ikvPriv->m_nDrawFrom = nToScreen*nCountsPerScreen;
    ikvPriv->m_nDrawTo = nCountsPerScreen-1 + ikvPriv->m_nDrawFrom;
    ikvPriv->m_bIsNeedDoLayout = true;
    ikvPriv->m_bIsNeedShowItemInfo = true;
    ikvPriv->m_bSendHighLighted = true;
}

void GCtrlIconView::emitSelected ( GCtrlIconViewItem* pItem )
{
    selected_pi.emit ( ikvPriv->m_nCurItemIndex );
    selected_pI.emit ( pItem );
}

void GCtrlIconView::setFocusInImage ( const GImage& img )
{
    ikvPriv->m_imgFocusIn.setImage ( img );
}

void GCtrlIconView::setFocusOutImage ( const GImage& img )
{
    ikvPriv->m_imgFocusOut.setImage ( img );
}

void GCtrlIconView::slotFocusChangedTo()
{
    if ( hasFocus() )
    {
        focusChangedTo_pI.emit ( ikvPriv->m_itemList.at ( ikvPriv->m_nCurItemIndex ) );
        focusChangedTo_pi.emit ( ikvPriv->m_nCurItemIndex );
        ikvPriv->m_itemList.at ( ikvPriv->m_nCurItemIndex )->getTxt()->startScroll();
    }
}

void GCtrlIconView::slotLoseFocus()
{
    ikvPriv->m_itemList.at ( ikvPriv->m_nCurItemIndex )->getTxt()->stopScroll();
}

void GCtrlIconView::slotGetFocus()
{
    if ( ikvPriv->m_itemList.isEmpty() )
        return;
    ikvPriv->m_itemList.at ( ikvPriv->m_nCurItemIndex )->setFocus();
    ikvPriv->m_bIsNeedShowItemInfo = true;
    highlighted_pI.emit ( ikvPriv->m_itemList.at ( ikvPriv->m_nCurItemIndex ) );
    highlighted_pi.emit ( ikvPriv->m_nCurItemIndex );
}

void GCtrlIconView::slotItemClicked ( const GCtrlIconViewItem* pItem )
{
    int nCurItemIndex = ikvPriv->m_itemList.findRef ( pItem );
    if ( -1 != nCurItemIndex )
    {
        selected_pI.emit ( ikvPriv->m_itemList.at ( nCurItemIndex ) );
        selected_pi.emit ( nCurItemIndex );
    }
}

bool GCtrlIconView::moveFocusLeft()
{
    if ( ikvPriv->m_itemList.count() >1 && ikvPriv->m_nCurItemIndex>0 )
    {
        moveFocus ( ikvPriv->m_nCurItemIndex-1 );
        return true;
    }
    else
    {
        return false;
    }
}

bool GCtrlIconView::moveFocusRight()
{
    if ( ikvPriv->m_itemList.count() >1 && ( unsigned int ) ikvPriv->m_nCurItemIndex<ikvPriv->m_itemList.count()-1 )
    {
        moveFocus ( ikvPriv->m_nCurItemIndex+1 );
        return true;
    }

    return false;
}

bool GCtrlIconView::moveFocusUp()
{
    if ( (int)ikvPriv->m_nCurItemIndex -  ikvPriv->m_nColumnNums >=0 )
    {
        moveFocus ( ikvPriv->m_nCurItemIndex-ikvPriv->m_nColumnNums );
        return true;
    }

    return false;
}

bool GCtrlIconView::moveFocusDown()
{
    if ( ikvPriv->m_nCurItemIndex + ikvPriv->m_nColumnNums <  ikvPriv->m_itemList.count() )
    {
        moveFocus ( ikvPriv->m_nCurItemIndex+ikvPriv->m_nColumnNums );
        return true;
    }
    else if ( ikvPriv->m_nCurItemIndex !=  ikvPriv->m_itemList.count()-1
              && ikvPriv->m_itemList.count()-ikvPriv->m_nCurItemIndex > ikvPriv->m_itemList.count() %ikvPriv->m_nColumnNums )
    {

        moveFocus ( ikvPriv->m_itemList.count()-1 );
        return true;
    }

    return false;
}

void GCtrlIconView::moveFocus ( int toIndex )
{
    int fromIndex = ikvPriv->m_nCurItemIndex;
    int nCountsPerScreen=ikvPriv->m_nColumnNums*ikvPriv->m_nRowNums;
    int nFromScreen=fromIndex/nCountsPerScreen;
    int nToScreen=toIndex/nCountsPerScreen;
    if ( nFromScreen != nToScreen )
    {
        ikvPriv->m_bIsNeedDoLayout = true;
        ikvPriv->m_nDrawFrom = nToScreen*nCountsPerScreen;
        ikvPriv->m_nDrawTo = ikvPriv->m_nDrawFrom+nCountsPerScreen-1;
    }

    ikvPriv->m_itemList.at ( fromIndex )->getTxt()->stopScroll();
    ikvPriv->m_itemList.at ( fromIndex )->getTxt()->setVisible ( false );

    if ( !ikvPriv->m_bIsNeedDoLayout )
    {
        ikvPriv->m_itemList.at ( fromIndex )->update();
        ikvPriv->m_itemList.at ( toIndex )->update();
    }
    else
    {
        update();
    }

    highlighted_pI.emit ( ikvPriv->m_itemList.at ( toIndex ) );
    highlighted_pi.emit ( toIndex );
    ikvPriv->m_nCurItemIndex = toIndex;
    ikvPriv->m_bIsNeedShowItemInfo = true;
}
int GCtrlIconView::getMaxIconHeight()
{
    return ikvPriv->m_nMaxHofItem;
}
int GCtrlIconView::getMaxIconWidth()
{
    return ikvPriv->m_nMaxWofItem;
}

// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
