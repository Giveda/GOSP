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

#include <gCtrlStyle.h>
#include "gCtrlListBox.h"
#include <gConstDefine.h>
#include <gGlobal.h>

GCtrlListBoxItem::GCtrlListBoxItem ( const GString& str, GCtrlForm* frm, GMItem* parent, const char* name )
    :GMCtrlItem ( frm, parent, name ), 
    m_txt ( str, frm, this, "listBoxText" )
{
    m_txt.setTextFlags ( Giveda::AlignVCenter );
}

void GCtrlListBoxItem::paintEvent ( GPainter& p )
{
    m_txt.draw ( p );
}

bool GCtrlListBoxItem::fwKeyPressEvent ( GKeyEvent *e )
{
    bool bRetVal = true;
    switch ( e->key() )
    {
    case Giveda::Key_Return:
        {
            GCtrlListBox* pBox = ( GCtrlListBox* ) parent();
            pBox->emitSelected ( this );
        }
        break;
    default:
        bRetVal = false;
        break;
    }

    return bRetVal;
}

void GCtrlListBoxItem::setGeometry ( int x, int y, int w, int h )
{
    GMItem::setGeometry ( x,y,w,h );
    m_txt.setSize ( width(), height() );
}

GCtrlListBoxPixmap::GCtrlListBoxPixmap ( const GPixmap& pm, const GString& str, GCtrlForm* frm, GMItem* parent, const char* name )
    :GCtrlListBoxItem ( str, frm, parent, name ), m_pix ( pm, frm, this, "listBoxPixmap" )
{
}

void GCtrlListBoxPixmap::paintEvent ( GPainter& p )
{
    m_pix.draw ( p );
    m_txt.draw ( p );
}

void GCtrlListBoxPixmap::setGeometry ( int x, int y, int w, int h )
{
    GMItem::setGeometry ( x,y,w,h );
    
    int nTmp = height() - m_pix.height();
    if ( nTmp > 1 )
    {
        m_pix.setY ( nTmp/2 );
    }
    m_txt.setGeometry ( m_pix.width(), 0,  width()-m_pix.width(), height() );
}

class GCtrlListBoxSelf
{
public:
    GCtrlListBoxSelf ( GCtrlForm* frm, GMItem* parent, GCtrlListBox *lb)
        :m_imgFocusIn ( frm, parent, "listBoxFocusIn" ), m_imgFocusOut ( frm, parent, "listBoxFocusOut" ), m_timerForShowItemInfo ( lb ), m_itemTxtColor ( 255,255,255 ), m_itemTxtFont ( "Sans", 20 )
    {}
    GPtrList<GCtrlListBoxItem> m_itemList;
    GMImage m_imgFocusIn;
    GMImage m_imgFocusOut;
    GTimer m_timerForShowItemInfo;
    unsigned int m_nCurItemIndex;
    bool m_bIsNeedDoLayout;
    bool m_bIsNeedShowItemInfo;
    bool m_bSendHighLighted;
    unsigned int m_nDrawFrom;
    unsigned int m_nDrawTo;
    unsigned int m_nMaxWofItem;
    unsigned int m_nMaxHofItem;
    int m_nColumnNums;
    int m_nRowNums;
    GColor m_itemTxtColor;
    GFont m_itemTxtFont;
    bool m_bScrollEnabled;
    int m_nPreviousItemKey;
    int m_nNextItemKey;
};

GCtrlListBox::GCtrlListBox ( GCtrlForm* frm, GMItem* parent, const char* name )
    :GMContainerItem ( frm, parent, name )
{
    lbSpp = new GCtrlListBoxSelf(frm, parent, this );
    frm->appendItem ( this );
    lbSpp->m_nDrawFrom = 0;
    lbSpp->m_nColumnNums = 1;
    setRowNums ( 3 );

    connect ( &lbSpp->m_timerForShowItemInfo,
              lbSpp->m_timerForShowItemInfo.timeout, this, &GCtrlListBox::slotFocusChangedTo );
    connect ( this, this->loseFocus, this, &GCtrlListBox::slotLoseFocus );
    connect ( this, this->getFocus, this, &GCtrlListBox::slotGetFocus );

    lbSpp->m_nCurItemIndex = 0;
    lbSpp->m_bIsNeedDoLayout = true;
    lbSpp->m_bIsNeedShowItemInfo = true;
    lbSpp->m_bSendHighLighted = false;

    GCtrlDefaultAppStyle* pAppStyle = getDefaultAppStyle();
    GCtrlItemStyle* pStyle=NULL;
    while ( NULL== ( pStyle=pAppStyle->itemStyle ( className() ) ) )
    {
        pAppStyle->appendListBoxStyle();
    }
    lbSpp->m_imgFocusIn.setImage ( pStyle->pixmap ( lbSpp->m_imgFocusIn.name() ) );
    lbSpp->m_imgFocusOut.setImage ( pStyle->pixmap ( lbSpp->m_imgFocusOut.name() ) );
    
    lbSpp->m_itemList.setAutoDelete ( true );

    lbSpp->m_bScrollEnabled = true;
    lbSpp->m_nPreviousItemKey = Giveda::Key_Up;
    lbSpp->m_nNextItemKey = Giveda::Key_Down;
}

GCtrlListBox::~GCtrlListBox()
{
    lbSpp->m_itemList.clear();
    delete lbSpp;
}

void GCtrlListBox::paintEvent ( GPainter& p )
{
    if ( lbSpp->m_itemList.isEmpty() )
    {
        return;
    }

    int nDrawTo = lbSpp->m_itemList.count()-1<lbSpp->m_nDrawTo ? lbSpp->m_itemList.count()-1 : lbSpp->m_nDrawTo;
    GCtrlListBoxItem* pItem=NULL;
    if ( lbSpp->m_bIsNeedDoLayout )
    {
        lbSpp->m_itemList.at ( lbSpp->m_nDrawFrom )->setGeometry ( 0, 0, lbSpp->m_nMaxWofItem, lbSpp->m_nMaxHofItem );
        lbSpp->m_itemList.at ( lbSpp->m_nDrawFrom )->getTxt()->setColor ( lbSpp->m_itemTxtColor );
        lbSpp->m_itemList.at ( lbSpp->m_nDrawFrom )->getTxt()->setFont ( lbSpp->m_itemTxtFont );
        int nBottomEdge = lbSpp->m_nMaxHofItem* ( lbSpp->m_nRowNums-1 );
        for ( int j=lbSpp->m_nDrawFrom; j<nDrawTo; j++ )
        {
            pItem = lbSpp->m_itemList.at ( j+1 );
            if ( lbSpp->m_itemList.at ( j )->y() == nBottomEdge )
            {
                pItem->setGeometry ( lbSpp->m_itemList.at ( j )->x() +lbSpp->m_nMaxWofItem, 0,  lbSpp->m_nMaxWofItem, lbSpp->m_nMaxHofItem );
            }
            else
            {
                pItem->setGeometry ( lbSpp->m_itemList.at ( j )->x(), lbSpp->m_itemList.at ( j )->y() +lbSpp->m_nMaxHofItem, lbSpp->m_nMaxWofItem, lbSpp->m_nMaxHofItem );
            }
            pItem->getTxt()->setColor ( lbSpp->m_itemTxtColor );
            pItem->getTxt()->setFont ( lbSpp->m_itemTxtFont );
        }

        lbSpp->m_bIsNeedDoLayout = false;
    }

    if ( hasFocus() )
    {
        lbSpp->m_imgFocusIn.setGeometry ( lbSpp->m_itemList.at ( lbSpp->m_nCurItemIndex )->x(), lbSpp->m_itemList.at ( lbSpp->m_nCurItemIndex )->y(), lbSpp->m_nMaxWofItem, lbSpp->m_nMaxHofItem );
        lbSpp->m_imgFocusIn.draw ( p );
    }
    else
    {
        lbSpp->m_imgFocusOut.setGeometry ( lbSpp->m_itemList.at ( lbSpp->m_nCurItemIndex )->x(), lbSpp->m_itemList.at ( lbSpp->m_nCurItemIndex )->y(), lbSpp->m_nMaxWofItem, lbSpp->m_nMaxHofItem );
        lbSpp->m_imgFocusOut.draw ( p );
    }

    for ( int j=lbSpp->m_nDrawFrom; j<=nDrawTo; j++ )
    {
        pItem = lbSpp->m_itemList.at ( j );

        p.save();
        p.translate ( pItem->x(), pItem->y() );
        pItem->draw ( p );
        p.restore();
    }

    if ( lbSpp->m_bSendHighLighted )
    {
        highlighted_pI.emit ( lbSpp->m_itemList.at ( lbSpp->m_nCurItemIndex ) );
        highlighted_pi.emit ( lbSpp->m_nCurItemIndex );
        lbSpp->m_bSendHighLighted = false;
    }

    if ( lbSpp->m_bIsNeedShowItemInfo )
    {
        lbSpp->m_timerForShowItemInfo.start ( 1000, true );
        lbSpp->m_bIsNeedShowItemInfo = false;
    }
}

bool GCtrlListBox::fwKeyPressEvent ( GKeyEvent* e )
{
    mpFocus = lbSpp->m_itemList.at ( lbSpp->m_nCurItemIndex );
    if ( mpFocus )
    {
        if ( true == mpFocus->fwKeyPress ( e ) )
        {
            return true;
        }
    }

    int nKey=e->key();
    bool bRetVal = true;
    if ( nKey == lbSpp->m_nPreviousItemKey )
    {
        bRetVal = moveFocusUp();
        if ( true == bRetVal )
        {
            return true;
        }
    }
    if ( nKey == lbSpp->m_nNextItemKey )
    {
        bRetVal = moveFocusDown();
        if ( true == bRetVal )
        {
            return true;
        }
    }
    switch ( nKey )
    {
    case Giveda::Key_Left:
        bRetVal = moveFocusLeft();
        break;
    case Giveda::Key_Right:
        bRetVal = moveFocusRight();
        break;
    default:
        bRetVal = false;
        break;
    }

    return bRetVal;
}

void GCtrlListBox::insertItem ( GCtrlListBoxItem* pItem, int index )
{
    if ( index<0 )
    {
        lbSpp->m_itemList.append ( pItem );
    }
    else
    {
        lbSpp->m_itemList.insert ( index, pItem );
    }
}

void GCtrlListBox::removeItem ( int index )
{
    lbSpp->m_itemList.remove(index);
    lbSpp->m_bIsNeedDoLayout = true;
}

void GCtrlListBox::changeItem ( GCtrlListBoxItem *pItem, int  index )
{
    if ( index == -1 )
    {
        index = lbSpp->m_nCurItemIndex;
    }

    lbSpp->m_itemList.remove ( index );
    lbSpp->m_itemList.insert ( index, pItem );
}

void GCtrlListBox::setRowNums ( unsigned int num )
{
    lbSpp->m_nRowNums = num;
    lbSpp->m_nMaxHofItem = height() /lbSpp->m_nRowNums;
    lbSpp->m_nDrawTo = lbSpp->m_nColumnNums*lbSpp->m_nRowNums-1 + lbSpp->m_nDrawFrom;
}

void GCtrlListBox::setColumnNums ( unsigned int num )
{
    lbSpp->m_nColumnNums = num;
    lbSpp->m_nMaxWofItem = width() /lbSpp->m_nColumnNums;
    lbSpp->m_nDrawTo = lbSpp->m_nColumnNums*lbSpp->m_nRowNums-1 + lbSpp->m_nDrawFrom;
}

GCtrlListBoxItem* GCtrlListBox::item ( int index )
{
    if ( -1 == index )
    {
        index = lbSpp->m_nCurItemIndex;
    }
    return lbSpp->m_itemList.at ( index );
}

unsigned int GCtrlListBox::count()
{
    return lbSpp->m_itemList.count();
}

void GCtrlListBox::clear()
{
    lbSpp->m_nDrawFrom = 0;
    lbSpp->m_nDrawTo = lbSpp->m_nColumnNums*lbSpp->m_nRowNums-1 + lbSpp->m_nDrawFrom;
    lbSpp->m_nCurItemIndex = 0;
    lbSpp->m_bIsNeedDoLayout = true;
    lbSpp->m_bIsNeedShowItemInfo = true;
    lbSpp->m_bSendHighLighted = true;

    lbSpp->m_itemList.clear();
}

void GCtrlListBox::setFocusInImage ( const GImage& img )
{
    lbSpp->m_imgFocusIn.setImage ( img );
}

void GCtrlListBox::setFocusOutImage ( const GImage& img )
{
    lbSpp->m_imgFocusOut.setImage ( img );
}

void GCtrlListBox::setCurItemIndex ( const int nIndex )
{
    lbSpp->m_nCurItemIndex = nIndex;
    int nCountsPerScreen=lbSpp->m_nColumnNums*lbSpp->m_nRowNums;
    int nToScreen=lbSpp->m_nCurItemIndex/nCountsPerScreen;
    lbSpp->m_nDrawFrom = nToScreen*nCountsPerScreen;
    lbSpp->m_nDrawTo = nCountsPerScreen-1 + lbSpp->m_nDrawFrom;
    lbSpp->m_bIsNeedDoLayout = true;
    lbSpp->m_bIsNeedShowItemInfo = true;
    lbSpp->m_bSendHighLighted = true;
}

void GCtrlListBox::emitSelected ( GCtrlListBoxItem* pItem )
{
    selected_pi.emit ( lbSpp->m_nCurItemIndex );
    selected_pI.emit ( pItem );
}

void GCtrlListBox::slotFocusChangedTo()
{
    if ( hasFocus() )
    {
        focusChangedTo_pi.emit ( lbSpp->m_nCurItemIndex );
        focusChangedTo_pI.emit ( lbSpp->m_itemList.at ( lbSpp->m_nCurItemIndex ) );
        if ( lbSpp->m_bScrollEnabled )
        {
            lbSpp->m_itemList.at ( lbSpp->m_nCurItemIndex )->getTxt()->startScroll();
        }
    }
}

void GCtrlListBox::slotLoseFocus()
{
    if ( lbSpp->m_itemList.isEmpty() )
    {
        return;
    }
    
    if ( lbSpp->m_bScrollEnabled )
    {
        lbSpp->m_itemList.at ( lbSpp->m_nCurItemIndex )->getTxt()->stopScroll();
    }
}

void GCtrlListBox::slotGetFocus()
{
    if ( lbSpp->m_itemList.isEmpty() )
    {
        return;
    }
    
    lbSpp->m_itemList.at ( lbSpp->m_nCurItemIndex )->setFocus();
    lbSpp->m_bIsNeedShowItemInfo = true;
    highlighted_pI.emit ( lbSpp->m_itemList.at ( lbSpp->m_nCurItemIndex ) );
    highlighted_pi.emit ( lbSpp->m_nCurItemIndex );
}

bool GCtrlListBox::moveFocusLeft()
{
    int nIndex = lbSpp->m_nCurItemIndex -lbSpp->m_nRowNums;
    if ( lbSpp->m_itemList.at ( nIndex ) )
    {
        moveFocus ( nIndex );
        return true;
    }

    return false;
}

bool GCtrlListBox::moveFocusRight()
{
    int nIndex = lbSpp->m_nCurItemIndex +lbSpp->m_nRowNums;
    if ( lbSpp->m_itemList.at ( nIndex ) )
    {
        moveFocus ( nIndex );
        return true;
    }

    return false;
}

bool GCtrlListBox::moveFocusUp()
{
    int nIndex = lbSpp->m_nCurItemIndex -1;
    if ( lbSpp->m_itemList.at ( nIndex ) )
    {
        moveFocus ( nIndex );
        return true;
    }

    return false;
}

bool GCtrlListBox::moveFocusDown()
{
    int nIndex = lbSpp->m_nCurItemIndex +1;
    if( lbSpp->m_itemList.at(nIndex) )
    {
        moveFocus( nIndex );
        return true;
    }

    return false;
}

void GCtrlListBox::moveFocus ( int toIndex )
{
    int fromIndex = lbSpp->m_nCurItemIndex;
    int nCountsPerScreen=lbSpp->m_nColumnNums*lbSpp->m_nRowNums;
    int nFromScreen=fromIndex/nCountsPerScreen;
    int nToScreen=toIndex/nCountsPerScreen;
    if ( nFromScreen != nToScreen )
    {
        lbSpp->m_bIsNeedDoLayout = true;
        lbSpp->m_nDrawFrom = nToScreen*nCountsPerScreen;
        lbSpp->m_nDrawTo = lbSpp->m_nDrawFrom+nCountsPerScreen-1;
    }

    lbSpp->m_itemList.at ( fromIndex )->getTxt()->stopScroll();
    lbSpp->m_itemList.at ( toIndex )->setFocus();
    if ( lbSpp->m_bIsNeedDoLayout )
    {
        update();
    }

    highlighted_pI.emit ( lbSpp->m_itemList.at ( toIndex ) );
    highlighted_pi.emit ( toIndex );
    lbSpp->m_nCurItemIndex = toIndex;
    lbSpp->m_bIsNeedShowItemInfo = true;
}
int GCtrlListBox::getMaxIconHeight()
{
    return lbSpp->m_nMaxHofItem;
}
int GCtrlListBox::getMaxIconWidth()
{
    return lbSpp->m_nMaxWofItem;
}
int GCtrlListBox::getCurItemIndex()
{
    return lbSpp->m_nCurItemIndex;
}
void GCtrlListBox::setItemTextColor ( GColor& c )
{
    lbSpp->m_itemTxtColor=c;
}
void GCtrlListBox::setItemTextFont ( GFont f )
{
    lbSpp->m_itemTxtFont = f;
}
GColor GCtrlListBox::getItemTextColor()
{
    return lbSpp->m_itemTxtColor;
}
GFont GCtrlListBox::getItemTextFont()
{
    return lbSpp->m_itemTxtFont;
}

void GCtrlListBox::setItemScrollEnabled ( bool enabled )
{
    lbSpp->m_bScrollEnabled = enabled;
}

void GCtrlListBox::setPreviousItemKey ( int nKey )
{
    lbSpp->m_nPreviousItemKey = nKey;
}

void GCtrlListBox::setNextItemKey ( int nKey )
{
    lbSpp->m_nNextItemKey = nKey;
}

// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
// have a nice day ^_^
