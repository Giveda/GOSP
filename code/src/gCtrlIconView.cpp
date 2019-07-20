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

class GCtrlIconViewFles
{
public:
    GCtrlIconViewFles( GCtrlForm* frm, GMItem* parent, GCtrlIconView *ikv )
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
    ikvFhl = new GCtrlIconViewFles(frm, parent, this);
    frm->appendItem ( this );
    ikvFhl->m_nDrawFrom = 0;
    ikvFhl->m_nRowNums = 3;
    setColumnNums ( 3 );
    setRowNums ( 3 );

    connect ( &ikvFhl->m_timerForShowItemInfo,
              ikvFhl->m_timerForShowItemInfo.timeout, this, &GCtrlIconView::slotFocusChangedTo );
    connect ( this, this->loseFocus, this, &GCtrlIconView::slotLoseFocus );
    connect ( this, this->getFocus, this, &GCtrlIconView::slotGetFocus );

    ikvFhl->m_nCurItemIndex = 0;
    ikvFhl->m_bIsNeedDoLayout = true;
    ikvFhl->m_bIsNeedShowItemInfo = true;
    ikvFhl->m_bSendHighLighted = false;

    GCtrlDefaultAppStyle* pAppStyle = getDefaultAppStyle();
    GCtrlItemStyle* pStyle=NULL;
    while ( NULL== ( pStyle=pAppStyle->itemStyle ( className() ) ) )
    {
        pAppStyle->appendIconViewStyle();
    }

    ikvFhl->m_imgFocusIn.setImage ( pStyle->pixmap ( ikvFhl->m_imgFocusIn.name() ) );
    ikvFhl->m_imgFocusOut.setImage ( pStyle->pixmap ( ikvFhl->m_imgFocusOut.name() ) );

    ikvFhl->m_itemList.setAutoDelete ( true );
}

GCtrlIconView::~GCtrlIconView()
{
    ikvFhl->m_itemList.clear();
    delete ikvFhl;
}

void GCtrlIconView::paintEvent ( GPainter& p )
{
    if ( ikvFhl->m_itemList.isEmpty() )
    {
        return;
    }

    int nDrawTo = ikvFhl->m_itemList.count()-1<ikvFhl->m_nDrawTo ? ikvFhl->m_itemList.count()-1 : ikvFhl->m_nDrawTo;
    GCtrlIconViewItem* pItem=NULL;
    if ( ikvFhl->m_bIsNeedDoLayout )
    {
        ikvFhl->m_itemList.at ( ikvFhl->m_nDrawFrom )->setGeometry ( 0, 0, ikvFhl->m_nMaxWofItem, ikvFhl->m_nMaxHofItem );
        int nRightEdge = ikvFhl->m_nMaxWofItem* ( ikvFhl->m_nColumnNums-1 );
        for ( int j=ikvFhl->m_nDrawFrom; j<nDrawTo; j++ )
        {
            pItem = ikvFhl->m_itemList.at ( j+1 );
            if ( ikvFhl->m_itemList.at ( j )->x() == nRightEdge )
            {
                pItem->setGeometry ( 0, ikvFhl->m_itemList.at ( j )->y() +ikvFhl->m_nMaxHofItem, ikvFhl->m_nMaxWofItem, ikvFhl->m_nMaxHofItem );
            }
            else
            {
                pItem->setGeometry ( ikvFhl->m_itemList.at ( j )->x() +ikvFhl->m_nMaxWofItem, ikvFhl->m_itemList.at ( j )->y(), ikvFhl->m_nMaxWofItem, ikvFhl->m_nMaxHofItem );
            }
        }

        ikvFhl->m_bIsNeedDoLayout = false;
    }

    ikvFhl->m_itemList.at ( ikvFhl->m_nCurItemIndex )->getTxt()->setVisible ( true );

    for ( int j=ikvFhl->m_nDrawFrom; j<=nDrawTo; j++ )
    {
        pItem = ikvFhl->m_itemList.at ( j );

        p.save();
        p.translate ( pItem->x(), pItem->y() );
        pItem->draw ( p );
        p.restore();
    }

    GMImage* pIcon=ikvFhl->m_itemList.at ( ikvFhl->m_nCurItemIndex )->getIcon();
    if ( hasFocus() )
    {
        ikvFhl->m_imgFocusIn.setGeometry ( ikvFhl->m_itemList.at ( ikvFhl->m_nCurItemIndex )->x(), ikvFhl->m_itemList.at ( ikvFhl->m_nCurItemIndex )->y(), ikvFhl->m_nMaxWofItem, pIcon->height() );
        ikvFhl->m_imgFocusIn.draw ( p );
    }
    else
    {
        ikvFhl->m_imgFocusOut.setGeometry ( ikvFhl->m_itemList.at ( ikvFhl->m_nCurItemIndex )->x(), ikvFhl->m_itemList.at ( ikvFhl->m_nCurItemIndex )->y(), ikvFhl->m_nMaxWofItem, pIcon->height() );
        ikvFhl->m_imgFocusOut.draw ( p );
    }

    if ( ikvFhl->m_bSendHighLighted )
    {
        highlighted_pI.emit ( ikvFhl->m_itemList.at ( ikvFhl->m_nCurItemIndex ) );
        highlighted_pi.emit ( ikvFhl->m_nCurItemIndex );
        ikvFhl->m_bSendHighLighted = false;
    }

    if ( ikvFhl->m_bIsNeedShowItemInfo )
    {
        ikvFhl->m_timerForShowItemInfo.start ( 1000, true );
        ikvFhl->m_bIsNeedShowItemInfo = false;
    }
}

bool GCtrlIconView::fwKeyPressEvent ( GKeyEvent* e )
{
    mpFocus = ikvFhl->m_itemList.at ( ikvFhl->m_nCurItemIndex );
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
        ikvFhl->m_itemList.append ( pItem );
    }
    else
    {
        ikvFhl->m_itemList.insert ( index, pItem );
    }
}

void GCtrlIconView::removeItem ( int index )
{
    ikvFhl->m_itemList.remove ( index );
    ikvFhl->m_bIsNeedDoLayout = true;
}

void GCtrlIconView::changeItem ( GCtrlIconViewItem *pItem, int  index )
{
    if ( index == -1 )
    {
        index = ikvFhl->m_nCurItemIndex;
    }

    ikvFhl->m_itemList.remove ( index );
    ikvFhl->m_itemList.insert ( index, pItem );
}

void GCtrlIconView::setRowNums ( unsigned int num )
{
    ikvFhl->m_nRowNums = num;
    ikvFhl->m_nMaxHofItem = height() /ikvFhl->m_nRowNums;
    ikvFhl->m_nDrawTo = ikvFhl->m_nColumnNums*ikvFhl->m_nRowNums-1 + ikvFhl->m_nDrawFrom;
}

void GCtrlIconView::setColumnNums ( unsigned int num )
{
    ikvFhl->m_nColumnNums = num;
    ikvFhl->m_nMaxWofItem = width() /ikvFhl->m_nColumnNums;
    ikvFhl->m_nDrawTo = ikvFhl->m_nColumnNums*ikvFhl->m_nRowNums-1 + ikvFhl->m_nDrawFrom;
}

GCtrlIconViewItem* GCtrlIconView::item ( int index )
{
    return ikvFhl->m_itemList.at ( index );
}

unsigned int GCtrlIconView::count()
{
    return ikvFhl->m_itemList.count();
}

void GCtrlIconView::clear()
{
    ikvFhl->m_nDrawFrom = 0;
    ikvFhl->m_nDrawTo = ikvFhl->m_nColumnNums*ikvFhl->m_nRowNums-1 + ikvFhl->m_nDrawFrom;
    ikvFhl->m_nCurItemIndex = 0;
    ikvFhl->m_bIsNeedDoLayout = true;
    ikvFhl->m_bIsNeedShowItemInfo = true;
    ikvFhl->m_bSendHighLighted = true;

    ikvFhl->m_itemList.clear();
}

void GCtrlIconView::setCurItemIndex ( const int nIndex )
{
    ikvFhl->m_nCurItemIndex = nIndex;
    int nCountsPerScreen=ikvFhl->m_nColumnNums*ikvFhl->m_nRowNums;
    int nToScreen=ikvFhl->m_nCurItemIndex/nCountsPerScreen;
    ikvFhl->m_nDrawFrom = nToScreen*nCountsPerScreen;
    ikvFhl->m_nDrawTo = nCountsPerScreen-1 + ikvFhl->m_nDrawFrom;
    ikvFhl->m_bIsNeedDoLayout = true;
    ikvFhl->m_bIsNeedShowItemInfo = true;
    ikvFhl->m_bSendHighLighted = true;
}

void GCtrlIconView::emitSelected ( GCtrlIconViewItem* pItem )
{
    selected_pi.emit ( ikvFhl->m_nCurItemIndex );
    selected_pI.emit ( pItem );
}

void GCtrlIconView::setFocusInImage ( const GImage& img )
{
    ikvFhl->m_imgFocusIn.setImage ( img );
}

void GCtrlIconView::setFocusOutImage ( const GImage& img )
{
    ikvFhl->m_imgFocusOut.setImage ( img );
}

void GCtrlIconView::slotFocusChangedTo()
{
    if ( hasFocus() )
    {
        focusChangedTo_pI.emit ( ikvFhl->m_itemList.at ( ikvFhl->m_nCurItemIndex ) );
        focusChangedTo_pi.emit ( ikvFhl->m_nCurItemIndex );
        ikvFhl->m_itemList.at ( ikvFhl->m_nCurItemIndex )->getTxt()->startScroll();
    }
}

void GCtrlIconView::slotLoseFocus()
{
    ikvFhl->m_itemList.at ( ikvFhl->m_nCurItemIndex )->getTxt()->stopScroll();
}

void GCtrlIconView::slotGetFocus()
{
    if ( ikvFhl->m_itemList.isEmpty() )
        return;
    ikvFhl->m_itemList.at ( ikvFhl->m_nCurItemIndex )->setFocus();
    ikvFhl->m_bIsNeedShowItemInfo = true;
    highlighted_pI.emit ( ikvFhl->m_itemList.at ( ikvFhl->m_nCurItemIndex ) );
    highlighted_pi.emit ( ikvFhl->m_nCurItemIndex );
}

void GCtrlIconView::slotItemClicked ( const GCtrlIconViewItem* pItem )
{
    int nCurItemIndex = ikvFhl->m_itemList.findRef ( pItem );
    if ( -1 != nCurItemIndex )
    {
        selected_pI.emit ( ikvFhl->m_itemList.at ( nCurItemIndex ) );
        selected_pi.emit ( nCurItemIndex );
    }
}

bool GCtrlIconView::moveFocusLeft()
{
    if ( ikvFhl->m_itemList.count() >1 && ikvFhl->m_nCurItemIndex>0 )
    {
        moveFocus ( ikvFhl->m_nCurItemIndex-1 );
        return true;
    }
    else
    {
        return false;
    }
}

bool GCtrlIconView::moveFocusRight()
{
    if ( ikvFhl->m_itemList.count() >1 && ( unsigned int ) ikvFhl->m_nCurItemIndex<ikvFhl->m_itemList.count()-1 )
    {
        moveFocus ( ikvFhl->m_nCurItemIndex+1 );
        return true;
    }

    return false;
}

bool GCtrlIconView::moveFocusUp()
{
    if ( (int)ikvFhl->m_nCurItemIndex -  ikvFhl->m_nColumnNums >=0 )
    {
        moveFocus ( ikvFhl->m_nCurItemIndex-ikvFhl->m_nColumnNums );
        return true;
    }

    return false;
}

bool GCtrlIconView::moveFocusDown()
{
    if ( ikvFhl->m_nCurItemIndex + ikvFhl->m_nColumnNums <  ikvFhl->m_itemList.count() )
    {
        moveFocus ( ikvFhl->m_nCurItemIndex+ikvFhl->m_nColumnNums );
        return true;
    }
    else if ( ikvFhl->m_nCurItemIndex !=  ikvFhl->m_itemList.count()-1
              && ikvFhl->m_itemList.count()-ikvFhl->m_nCurItemIndex > ikvFhl->m_itemList.count() %ikvFhl->m_nColumnNums )
    {

        moveFocus ( ikvFhl->m_itemList.count()-1 );
        return true;
    }

    return false;
}

void GCtrlIconView::moveFocus ( int toIndex )
{
    int fromIndex = ikvFhl->m_nCurItemIndex;
    int nCountsPerScreen=ikvFhl->m_nColumnNums*ikvFhl->m_nRowNums;
    int nFromScreen=fromIndex/nCountsPerScreen;
    int nToScreen=toIndex/nCountsPerScreen;
    if ( nFromScreen != nToScreen )
    {
        ikvFhl->m_bIsNeedDoLayout = true;
        ikvFhl->m_nDrawFrom = nToScreen*nCountsPerScreen;
        ikvFhl->m_nDrawTo = ikvFhl->m_nDrawFrom+nCountsPerScreen-1;
    }

    ikvFhl->m_itemList.at ( fromIndex )->getTxt()->stopScroll();
    ikvFhl->m_itemList.at ( fromIndex )->getTxt()->setVisible ( false );

    if ( !ikvFhl->m_bIsNeedDoLayout )
    {
        ikvFhl->m_itemList.at ( fromIndex )->update();
        ikvFhl->m_itemList.at ( toIndex )->update();
    }
    else
    {
        update();
    }

    highlighted_pI.emit ( ikvFhl->m_itemList.at ( toIndex ) );
    
    highlighted_pi.emit ( toIndex );
    ikvFhl->m_nCurItemIndex = toIndex;
    ikvFhl->m_bIsNeedShowItemInfo = true;
}

int GCtrlIconView::getMaxIconHeight()
{
    return ikvFhl->m_nMaxHofItem;
}

int GCtrlIconView::getMaxIconWidth()
{
    return ikvFhl->m_nMaxWofItem;
}

// have a nice day ^_^
